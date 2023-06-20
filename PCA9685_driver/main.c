
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>

int pca_minor =   0;

struct pca_dev {
	struct i2c_client * client;
	struct miscdevice pca_miscdevice;
	char name[8];
};

/* User is reading data from /dev/pcaXX */
static ssize_t pca_read_file(struct file *file, char __user *userbuf,
                               size_t count, loff_t *ppos)
{
	int expval, size;
	char buf[3];
	struct pca_dev * pca;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	expval = i2c_smbus_read_byte(pca->client);
	if (expval < 0)
		return -EFAULT;

	/* 
     * converts expval in 2 characters (2bytes) + null value (1byte)
	 * The values converted are char values (FF) that match with the hex
	 * int(s32) value of the expval variable.
	 * if we want to get the int value again, we have to
	 * do Kstrtoul(). We convert 1 byte int value to
	 * 2 bytes char values. For instance 255 (1 int byte) = FF (2 char bytes).
	 */
	size = sprintf(buf, "%02x", expval);

	/* 
         * replace NULL by \n. It is not needed to have the char array
	 * ended with \0 character.
	 */
	buf[size] = '\n';

	/* send size+1 to include the \n character */
	if(*ppos == 0){
		if(copy_to_user(userbuf, buf, size+1)){
			pr_info("Failed to return led_value to user space\n");
			return -EFAULT;
		}
		*ppos+=1;
		return size+1;
	}

	return 0;
}

/* Writing from the terminal command line, \n is added */
static ssize_t pca_write_file(struct file *file, const char __user *userbuf,
                                   size_t count, loff_t *ppos)
{
	int ret;
	unsigned long val;
	char buf[4];
	struct pca_dev * pca;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	dev_info(&pca->client->dev, 
		 "pca_write_file entered on %s\n", pca->name);

	dev_info(&pca->client->dev,
		 "we have written %zu characters\n", count); 

	if(copy_from_user(buf, userbuf, count)) {
		dev_err(&pca->client->dev, "Bad copied value\n");
		return -EFAULT;
	}

	buf[count-1] = '\0';

	/* convert the string to an unsigned long */
	ret = kstrtoul(buf, 0, &val);
	if (ret)
		return -EINVAL;

	dev_info(&pca->client->dev, "the value is %lu\n", val);

	ret = i2c_smbus_write_byte(pca->client, val);
	if (ret < 0)
		dev_err(&pca->client->dev, "the device is not found\n");

	dev_info(&pca->client->dev, 
		 "pca_write_file exited on %s\n", pca->name);

	return count;
}

static const struct file_operations pca_fops = {
	.owner = THIS_MODULE,
	.read = pca_read_file,
	.write = pca_write_file,
};

static int pca_probe(struct i2c_client * client,
		const struct i2c_device_id * id)
{
	struct pca_dev * pca;

	/* Allocate new structure representing device */
	pca = devm_kzalloc(&client->dev, sizeof(struct pca_dev), GFP_KERNEL);

	/* Store pointer to the device-structure in bus device context */
	i2c_set_clientdata(client,pca);

	/* Store pointer to I2C device/client */
	pca->client = client;

	/* Initialize the misc device */
	sprintf(pca->name, "pca9685"); 
	dev_info(&client->dev, 
		 "pca_probe is entered on %s\n", pca->name);

	pca->pca_miscdevice.name = pca->name;
	pca->pca_miscdevice.minor = pca_minor;
	pca->pca_miscdevice.fops = &pca_fops;

	/* Register misc device */
	return misc_register(&pca->pca_miscdevice);

	dev_info(&client->dev, 
		 "pca_probe is exited on %s\n", pca->name);

	return 0;
}

static int pca_remove(struct i2c_client * client)
{
	struct pca_dev * pca;

	/* Get device structure from bus device context */	
	pca = i2c_get_clientdata(client);

	dev_info(&client->dev, 
		 "pca_remove is entered on %s\n", pca->name);

	/* Deregister misc device */
	misc_deregister(&pca->pca_miscdevice);

	dev_info(&client->dev, 
		 "pca_remove is exited on %s\n", pca->name);

	return 0;
}

static const struct of_device_id pca_dt_ids[] = {
	{ .compatible = "arrow,pca", },
	{ }
};
MODULE_DEVICE_TABLE(of, pca_dt_ids);

static const struct i2c_device_id i2c_ids[] = {
	{ .name = "pca", },
	{ }
};
MODULE_DEVICE_TABLE(i2c, i2c_ids);

static struct i2c_driver pca_driver = {
	.driver = {
		.name = "pca",
		.owner = THIS_MODULE,
		.of_match_table = pca_dt_ids,
	},
	.probe = pca_probe,
	.remove = pca_remove,
	.id_table = i2c_ids,
};

module_i2c_driver(pca_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cristian Csordas");
MODULE_DESCRIPTION("This is a driver that controls PCA9685");


