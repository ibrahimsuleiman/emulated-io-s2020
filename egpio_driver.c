#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include<asm/ioctl.h>

#include"gpio.h"

#define EGPIO_MAJOR 42
#define EGPIO_MAX_MINORS 4

#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3



struct egpio_port {
	uint8_t pins; /* pin values 1, HIGH, 0, LOW*/
	uint8_t pinmodes; /* each bit correspond to the mode of the corresponding pin*/
};



struct egpio_device {
	struct cdev cdev;
	struct egpio_port port;
};



/* EGPIO ports A through D*/
struct egpio_device devs[EGPIO_MAX_MINORS];

static int egpio_open(struct inode *inode, struct file *filp)
{

	struct egpio_device *egpio = container_of(inode->i_cdev, struct egpio_device, cdev);
	filp->private_data = egpio;

	/* now we initialize the gpio device */

	printk(KERN_ALERT "EGPIO device open\n");
	return 0;
}

static ssize_t egpio_read(struct file *filp, char __user *user_buff, size_t size, loff_t *offset)
{
	struct egpio_device *egpio = (struct egpio_device *)filp->private_data;
	
	uint8_t port_value = egpio->port.pins;
	
	if(copy_to_user(user_buff, &port_value, sizeof(port_value)))
		return -EFAULT;	
	return 0;
}

static ssize_t egpio_write(struct file *filp, const char __user *user_buff, size_t size, loff_t *offset)
{
	
	
	uint8_t value = 0;
	
	struct egpio_device *egpio = (struct egpio_device *)filp->private_data;
	
	if(copy_from_user(&value, user_buff, sizeof(value)))
		return -EFAULT;
	
	egpio->port.pins = value;
	
	return 0;
}

static int egpio_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "Realeasing!\n");
	return 0;
}

static long egpio_ioctl(struct file *filp, unsigned int cmd, unsigned long argv)
{
	
	int i;
	
	struct egpio_device *egpio = (struct egpio_device *)filp->private_data;
	switch(cmd) {
		case IOC_RESET_GPIO:
			for(i = 0; i < EGPIO_MAX_MINORS; ++i) {
				devs[i].port.pinmodes = 0;
				devs[i].port.pins = 0;
			}
			break;
		case IOC_INIT_PORT:
			egpio->port.pinmodes = 0; 	/*00000000*/
			egpio->port.pins = 0;
			break;
		case IOC_SET_PORT_OUT:
			egpio->port.pinmodes = 255; 	/*11111111*/
			break;
		case IOC_SET_PORT_IN:
			egpio->port.pinmodes = 0;	/*00000000*/
			break;

		default:
			break;
	}


	return 0;


}

const struct file_operations egpio_fops = {
	.owner = THIS_MODULE,
	.open = egpio_open,
	.read = egpio_read,
	.write = egpio_write,
	.release = egpio_release,
	.unlocked_ioctl = egpio_ioctl,
};


int egpio_init(void)
{
	
	int i, err;
	
	printk(KERN_ALERT "egpio module loaded\n");
	err = register_chrdev_region(MKDEV(EGPIO_MAJOR, 0), EGPIO_MAX_MINORS, "egpio");
	if(err) {
		return err;
	}

	for(i = 0; i < EGPIO_MAX_MINORS; i++) {
		cdev_init(&devs[i].cdev, &egpio_fops);
		cdev_add(&devs[i].cdev, MKDEV(EGPIO_MAJOR, i), 1);
	}

	return 0;
}

void egpio_cleanup(void)
{
	int i;
	for(i = 0; i < EGPIO_MAX_MINORS; i++) {
		cdev_del(&devs[i].cdev);
	}
	unregister_chrdev_region(MKDEV(EGPIO_MAJOR, 0), EGPIO_MAX_MINORS);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ibrahim Suleiman");
MODULE_DESCRIPTION("Emulated GPIO driver");
module_init(egpio_init);
module_exit(egpio_cleanup);



