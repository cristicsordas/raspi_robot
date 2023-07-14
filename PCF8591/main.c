
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include "PCF8591.h"
#include "pcf_ioctl.h"


int pcf_minor =   0;
static uint8_t current_channel = 0;

static const uint8_t MAX_LIST_SIZE = 9;

struct Node {
	uint32_t value;
	struct Node *next;
};

static struct Node *head = NULL;

static void create_list(void)
{
	uint8_t count = 0;
	struct Node *new_elem = NULL; 

	PDEBUG("create elem %zu\n", sizeof(struct Node));

	head = kmalloc(sizeof(struct Node), GFP_KERNEL);
	head->value = 0;
	head->next = NULL;

	while(count++ < MAX_LIST_SIZE-1)
	{
		PDEBUG("create elem %zu\n", sizeof(struct Node));
		new_elem = kmalloc(sizeof(struct Node), GFP_KERNEL);
		new_elem->value = 0;
		new_elem->next = head;
		head = new_elem;
	}
}

static void empty_list(void)
{
	struct Node *ptr_runner = head;
	while (ptr_runner != NULL)
	{
		ptr_runner->value = 0;
		ptr_runner = ptr_runner->next;
	}
	
}

static void swap_elem(struct Node *node1, struct Node *node2)
{
	uint32_t temp = node1->value;
	node1->value = node2->value;
	node2->value = temp;
}

static void sort_list(void)
{
	bool is_swapped = false;
	struct Node *ptr_runner = head;
	struct Node *ptr_end = NULL;

	PDEBUG("sort the list\n");

	if(head == NULL)
		return;

	do
	{
		is_swapped = false; 
		ptr_runner = head;

		while(ptr_runner->next != ptr_end)
		{
			if(ptr_runner->value > ptr_runner->next->value)
			{
				swap_elem(ptr_runner, ptr_runner->next);
				is_swapped = true;
			}
			ptr_runner = ptr_runner->next;
		}
		ptr_end = ptr_runner;
	} while (is_swapped);

	PDEBUG("sort the list end\n");
}

static void clear_list(void)
{
	struct Node *current_elem = head;
	while(current_elem != NULL)
	{
		PDEBUG("free elem %du\n", current_elem->value);
		head = head->next;
		kfree(current_elem);
		current_elem = head;
	}
}

static uint32_t get_middle_value(void)
{
	uint32_t value = 0;
	struct Node* runner = head;
	struct Node* fast_runner = head;
	while (fast_runner != NULL && fast_runner->next != NULL)
	{
		runner = runner->next;
		fast_runner->next = fast_runner->next->next;
	}

	value = runner->value;

	return value;
}

void print_list(void)
{
	struct Node *current_elem = head;
	while(current_elem != NULL)
	{
		PDEBUG("list elem %du\n", current_elem->value);
		current_elem = current_elem->next;
	}
}

struct pcf_dev {
	struct i2c_client * client;
	struct miscdevice pcf_miscdevice;
	char name[8];
};

/* User is reading data from /dev/pcfXX */
static ssize_t pcf_read_file(struct file *file, char __user *userbuf,
                               size_t count, loff_t *ppos)
{
	int32_t expval = 0;
	uint32_t result = 0;
	uint32_t size = 0;
	uint32_t middle_value = 0;
	char buf_output[4];
	struct pcf_dev * pcf;
	struct Node *insert_elem;

	PDEBUG("read file\n");
	pcf = container_of(file->private_data,
			     struct pcf_dev, 
			     pcf_miscdevice);


	PDEBUG("read from channel %du\n", current_channel);
	empty_list();
	insert_elem = head;
	while(insert_elem != NULL)
	{
		expval = i2c_smbus_read_byte_data(pcf->client, COMMAND + current_channel);
		PDEBUG("i2c read %du\n", expval);
		if (expval < 0) {
			result = -EFAULT;
			goto exit;
		}
		
		insert_elem->value = (uint32_t)expval;
		insert_elem = insert_elem->next;
	}
	sort_list();
	print_list();

	middle_value = get_middle_value();
	buf_output[0] = middle_value & 0xFF;
	buf_output[1] = (middle_value >> 8) & 0xFF;
	buf_output[2] = (middle_value >> 16) & 0xFF;
	buf_output[3] = (middle_value >> 24) & 0xFF;
	size = sizeof(buf_output);

	if(copy_to_user(userbuf, buf_output, size)){
		pr_info("Failed to return i2c read value to user space\n");
		result = -EFAULT;
		goto exit;
	}
	*ppos = 0;

	result = size;

exit:
	empty_list();
	return result;
}

long pcf_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long retval = 0;
	uint8_t channel = 0;

	switch (cmd)
	{
	case PCF_IOC_SET_CHANNEL:
		PDEBUG("ioctl set all pwm\n");
		if(copy_from_user(&channel, (const void __user *)arg, sizeof(channel)) != 0)
        {
			PDEBUG("copy from user error\n");
            retval = -EFAULT;
        }
        else
        {
			current_channel = channel;
			PDEBUG("set current channel %du \n", current_channel);
        }
		break;
	default:
		break;
	}

	return retval;
}

static const struct file_operations pcf_fops = {
	.owner = THIS_MODULE,
	.read = pcf_read_file,
	.compat_ioctl = pcf_ioctl,
	.unlocked_ioctl = pcf_ioctl
};

static int pcf_probe(struct i2c_client * client,
		const struct i2c_device_id * id)
{
	struct pcf_dev * pcf;

	/* Allocate new structure representing device */
	pcf = devm_kzalloc(&client->dev, sizeof(struct pcf_dev), GFP_KERNEL);

	/* Store pointer to the device-structure in bus device context */
	i2c_set_clientdata(client,pcf);

	/* Store pointer to I2C device/client */
	pcf->client = client;

	/* Initialize the misc device */
	sprintf(pcf->name, "pcf8591"); 
	PDEBUG("pcf_probe is entered on %s\n", pcf->name);

	pcf->pcf_miscdevice.name = pcf->name;
	pcf->pcf_miscdevice.minor = pcf_minor;
	pcf->pcf_miscdevice.fops = &pcf_fops;

	create_list();

	/* Register misc device */
	return misc_register(&pcf->pcf_miscdevice);

	PDEBUG("pcf_probe is exited on %s\n", pcf->name);

	return 0;
}

static int pcf_remove(struct i2c_client * client)
{
	struct pcf_dev * pcf;

	clear_list();

	/* Get device structure from bus device context */	
	pcf = i2c_get_clientdata(client);

	PDEBUG("pcf_remove is entered on %s\n", pcf->name);

	/* Deregister misc device */
	misc_deregister(&pcf->pcf_miscdevice);

	PDEBUG("pcf_remove is exited on %s\n", pcf->name);

	return 0;
}

static const struct of_device_id pcf_dt_ids[] = {
	{ .compatible = "arrow,pcf", },
	{ }
};
MODULE_DEVICE_TABLE(of, pcf_dt_ids);

static const struct i2c_device_id i2c_ids[] = {
	{ .name = "pcf", },
	{ }
};
MODULE_DEVICE_TABLE(i2c, i2c_ids);

static struct i2c_driver pcf_driver = {
	.driver = {
		.name = "pcf",
		.owner = THIS_MODULE,
		.of_match_table = pcf_dt_ids,
	},
	.probe = pcf_probe,
	.remove = pcf_remove,
	.id_table = i2c_ids,
};

module_i2c_driver(pcf_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cristian Csordas");
MODULE_DESCRIPTION("This is a driver that controls PCF8591");


