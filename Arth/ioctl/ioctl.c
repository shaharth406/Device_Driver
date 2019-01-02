#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int32_t value=0;

dev_t dev=0;

static struct class *dev_class;
static struct cdev my_cdev;

static int my_driver_init(void);
static void my_driver_exit(void);
static int driver_open(struct inode *inode, struct file *file);
static int driver_release(struct inode *inode, struct file *file);
static ssize_t driver_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t driver_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg); 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = driver_read,
        .write          = driver_write,
        .open           = driver_open,
	.unlocked_ioctl = my_ioctl,
        .release        = driver_release,
};
 
static int driver_open(struct inode *inode, struct file *file)
{
        
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
 
static int driver_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
 
static ssize_t driver_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
      
        printk(KERN_INFO "Data Read successfully..\n");
        return 0;
}
static ssize_t driver_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Data Write successfully..\n");
        return 0;
}
 
static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
	
		case WR_VALUE:
			copy_from_user(&value,(int32_t*)arg,sizeof(value));
			printk(KERN_INFO "value = %d\n",value);
			break;
		case RD_VALUE:
			copy_to_user((int32_t*)arg,&value,sizeof(value));
			break;

	}
	return 0;

}


static int my_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "my_dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&my_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&my_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert successfully..!\n");
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
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove successfully..!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arth Shah");
MODULE_DESCRIPTION("Example of ioctl");
MODULE_VERSION("2.4");


































