
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include "PCA9695.h"

int pca_minor =   0;

struct pca_dev {
	struct i2c_client * client;
	struct miscdevice pca_miscdevice;
	char name[8];
};

struct i2c_pca_data {
	char command;
	char data;
};

/* User is reading data from /dev/pcaXX */
static ssize_t pca_read_file(struct file *file, char __user *userbuf,
                               size_t count, loff_t *ppos)
{
	uint32_t expval, size;
	char buf[3];
	struct pca_dev * pca;
	uint8_t MODE1 = 0x00;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	expval = i2c_smbus_read_byte_data(pca->client, MODE1);
	if (expval < 0)
		return -EFAULT;

	size = sprintf(buf, "%02x", expval);

	/* send size+1 to include the \n character */
	if(*ppos == 0){
		if(copy_to_user(userbuf, buf, size)){
			pr_info("Failed to return led_value to user space\n");
			return -EFAULT;
		}
		*ppos+=1;
		return size;
	}

	return 0;
}

/* Writing from the terminal command line, \n is added */
static ssize_t pca_write_file(struct file *file, const char __user *userbuf,
                                   size_t count, loff_t *ppos)
{
	int ret;
	struct pca_dev * pca;
	char buf[4];

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	PDEBUG("pca_write_file entered on %s\n", pca->name);

	PDEBUG("we have written %zu characters\n", count); 

	if(copy_from_user(buf, userbuf, count)) {
		PDEBUG("Bad copied value\n");
		return -EFAULT;
	}

	PDEBUG("the command is %d\n", buf[0]);
	PDEBUG("the value is %d\n", buf[1]);

	ret = i2c_smbus_write_byte_data(pca->client, buf[0], buf[1]);
	if (ret < 0)
		PDEBUG("the device is not found\n");

	PDEBUG("pca_write_file exited on %s\n", pca->name);

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
	PDEBUG("pca_probe is entered on %s\n", pca->name);

	pca->pca_miscdevice.name = pca->name;
	pca->pca_miscdevice.minor = pca_minor;
	pca->pca_miscdevice.fops = &pca_fops;

	/* Register misc device */
	return misc_register(&pca->pca_miscdevice);

	PDEBUG("pca_probe is exited on %s\n", pca->name);

	return 0;
}

static int pca_remove(struct i2c_client * client)
{
	struct pca_dev * pca;

	/* Get device structure from bus device context */	
	pca = i2c_get_clientdata(client);

	PDEBUG("pca_remove is entered on %s\n", pca->name);

	/* Deregister misc device */
	misc_deregister(&pca->pca_miscdevice);

	PDEBUG("pca_remove is exited on %s\n", pca->name);

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


