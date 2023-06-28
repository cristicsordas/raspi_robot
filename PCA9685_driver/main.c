
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include "PCA9695.h"
#include "pca_ioctl.h"
#include "constants.h"

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
	char buf[4];
	struct pca_dev * pca;
	uint8_t MODE1 = 0x00;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	expval = i2c_smbus_read_byte_data(pca->client, MODE1);
	if (expval < 0)
		return -EFAULT;
	
	buf[0] = expval & 0xFF;
	buf[1] = (expval >> 8) & 0xFF;
	buf[2] = (expval >> 16) & 0xFF;
	buf[3] = (expval >> 24) & 0xFF;
	size = sizeof(buf);

	if(copy_to_user(userbuf, buf, size)){
		pr_info("Failed to return i2c read value to user space\n");
		return -EFAULT;
	}
	*ppos = 0;

	return size;
}

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

long set_all_pwm(struct file *file, const uint16_t on_value, const uint16_t off_value)
{
	int ret;
	struct pca_dev * pca;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	ret = i2c_smbus_write_byte_data(pca->client, ALL_LED_ON_L, on_value & 0xFF);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, ALL_LED_ON_H, on_value >> 8);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, ALL_LED_OFF_L, off_value & 0xFF);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, ALL_LED_OFF_H, off_value >> 8);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	return 0;
}

long set_pwm(struct file *file, const uint8_t channel, uint16_t on_value, const uint16_t off_value)
{
	const uint8_t channel_offset = 4 * channel;
	int ret;
	struct pca_dev * pca;

	pca = container_of(file->private_data,
			     struct pca_dev, 
			     pca_miscdevice);

	ret = i2c_smbus_write_byte_data(pca->client, LED0_ON_L + channel_offset, on_value & 0xFF);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, LED0_ON_H + channel_offset, on_value >> 8);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, LED0_OFF_L + channel_offset, off_value & 0xFF);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, LED0_OFF_H + channel_offset, off_value >> 8);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	return 0;
}

long set_freqv(struct file *file, uint32_t freqv)
{
	struct pca_dev * pca;
	uint32_t oldmode = 0;
	uint32_t newmode = 0;
	int ret;

	pca = container_of(file->private_data,
				struct pca_dev, 
				pca_miscdevice);

	oldmode = i2c_smbus_read_byte_data(pca->client, MODE1);
	if (oldmode < 0)
		return -EFAULT;

  	newmode = (oldmode & 0x7F) | SLEEP;

	ret = i2c_smbus_write_byte_data(pca->client, MODE1, newmode);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, PRESCALE, freqv);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, MODE1, oldmode);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	ret = i2c_smbus_write_byte_data(pca->client, MODE1, oldmode | RESTART);
	if (ret < 0)
	{
		PDEBUG("device not found\n");
		return -EFAULT;
	}

	return 0;
}

long pca_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long retval = 0;
	struct pca_pwm pwm_val;
	struct pca_channel_pwm pwm_channel_val;
	uint32_t freqv = 0;

	switch (cmd)
	{
	case PCA_IOC_SET_ALL_PWM:
		PDEBUG("ioctl set all pwm\n");
		if(copy_from_user(&pwm_val, (const void __user *)arg, sizeof(pwm_val)) != 0)
        {
			PDEBUG("copy from user error\n");
            retval = -EFAULT;
        }
        else
        {
			PDEBUG("set_all_pwm\n");
            retval = set_all_pwm(filp, pwm_val.on_value, pwm_val.off_value);
			PDEBUG("set_all_pwm retval %zu\n", retval);
        }
		break;
	case PCA_IOC_SET_PWM_FREQV:
		PDEBUG("ioctl set_freqv\n");
		if(copy_from_user(&freqv, (const void __user *)arg, sizeof(freqv)) != 0)
        {
			PDEBUG("copy from user error\n");
            retval = -EFAULT;
        }
        else
        {
			PDEBUG("set_freqv\n");
            retval = set_freqv(filp, freqv);
			PDEBUG("set_freqv retval %zu\n", retval);
        }
		break;
	case PCA_IOC_SET_PWM_ON_CHANNEL:
		PDEBUG("ioctl set pwm\n");
		if(copy_from_user(&pwm_channel_val, (const void __user *)arg, sizeof(pwm_channel_val)) != 0)
        {
			PDEBUG("copy from user error\n");
            retval = -EFAULT;
        }
        else
        {
			PDEBUG("set_pwm\n");
            retval = set_pwm(filp, pwm_channel_val.channel, pwm_channel_val.on_value, pwm_channel_val.off_value);
			PDEBUG("set_pwm retval %zu\n", retval);
        }
		break;
		break;
	default:
		break;
	}
	return retval;
}

static const struct file_operations pca_fops = {
	.owner = THIS_MODULE,
	.read = pca_read_file,
	.write = pca_write_file,
	.compat_ioctl = pca_ioctl,
	.unlocked_ioctl = pca_ioctl
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


