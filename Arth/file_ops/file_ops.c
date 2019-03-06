#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/module.h>
#include<linux/cdev.h>

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"ARTH"
#define DRIVER_MODULE_DESC			"File operation cdev structure"
#define DRIVER_MODULE_VERSION			"V1.0"


dev_t dev=0;
static struct class *dev_class;
static struct cdev my_cdev;

static int my_driver_init(void);
static void my_driver_exit(void);
static int driver_open(struct inode *inode,struct file *file);
static int driver_release(struct inode *inode,struct file *file);
static ssize_t driver_read(struct file *filp,char __user *buf,size_t len,loff_t *off);
static ssize_t driver_write(struct file *filp,const char *buf,size_t len,loff_t *off);

static struct file_operations fops =
{
.owner = THIS_MODULE,
.read = driver_read,
.write = driver_write,
.open = driver_open,
.release = driver_release,
};


static int driver_open(struct inode *inode,struct file *file)
{

printk(KERN_INFO "Driver open function called..\n");
return 0;

}

static int driver_release(struct inode *inode,struct file *file)
{

printk(KERN_INFO "Driver release function called...\n");
return 0;
}

static ssize_t driver_read(struct file *filp,char __user *buf,size_t len,loff_t *off)
{

printk(KERN_INFO "Driver Read function called..\n");
return 0;
}

static ssize_t driver_write(struct file *filp,const char __user *buf,size_t len,loff_t *off)
{
printk(KERN_INFO "Driver Write function called...\n");
return len;

}

static int my_driver_init(void)
{
	/*Allocating Major Number*/
	if((alloc_chrdev_region(&dev,0,1,"my_dev"))<0)
	{
	printk(KERN_INFO "cannot allocate Major No \n");
	return -1;
	}
	printk(KERN_INFO "Major=%d Minor=%d",MAJOR(dev),MINOR(dev));

	/*creating cdev structure*/
	cdev_init(&my_cdev,&fops);

	/*Adding character device to the system*/
	if((cdev_add(&my_cdev,dev,1))<0)
	{
	printk(KERN_INFO "cannot add the device to the system\n");
	goto r_class;
	}

	/*creating struct class*/
	if((dev_class=class_create(THIS_MODULE,"my_class"))==NULL)
	{
	printk(KERN_INFO "cannot create struct class\n");
	goto r_class;
	}

	/*creating device*/
	if((device_create(dev_class,NULL,dev,NULL,"my_device"))==NULL)
	{
	printk(KERN_INFO "cannot create device \n");
	goto r_device;

	}

printk(KERN_INFO "Device Driver Insert Successfully..\n");
return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;

}

void my_driver_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "Device Driver Remove..\n");

}

module_init(my_driver_init);
module_exit(my_driver_exit);	

MODULE_LICENSE (GPL_LICENSE);
MODULE_AUTHOR  (DRIVER_MODULE_AUTHOR);
MODULE_VERSION (DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION (DRIVER_MODULE_DESC);



