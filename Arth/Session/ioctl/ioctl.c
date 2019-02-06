/******************************************************************************
@fileName : ioctl.c
@author   : Arth Shah
@teamLead : Rajesh Dommaraju 
@details  : It is a simple ioctl driver to load a module in the kernel   
@license  : SpanIdea Systems Pvt. Ltd. All rights reserved.
*******************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
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

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"ARTH"
#define DRIVER_MODULE_DESC			"Simple IOCTL driver"
#define DRIVER_MODULE_VERSION			"V1.0"

#define DEVICE_COUNT				1
/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
//int32_t value=0;
struct temp
{
int value1;
char str[20];
int value2;
};
dev_t dev=0;

struct temp val;

static struct class *dev_class;
static struct cdev my_cdev;
/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int my_driver_init(void);
static void my_driver_exit(void);
static int driver_open(struct inode *inode, struct file *file);
static int driver_release(struct inode *inode, struct file *file);
static ssize_t driver_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t driver_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long my_ioctl(struct file *file,unsigned int cmd,struct temp *arg); 

/*************************File operation structure*****************************/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = driver_read,
        .write          = driver_write,
        .open           = driver_open,
	.unlocked_ioctl = my_ioctl,
        .release        = driver_release,
};
 
/*************************************************************************
Function Name   :driver_open
Description     :this function open the device and allocate required resources
Input_param     :1)pointer to inode structure
		:2)pointer to the file structure
Output_param    :1)None
**************************************************************************/
static int driver_open(struct inode *inode, struct file *file)
{
        
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
/*************************************************************************
function Name   :driver_release
Description     :this function close the device and de-allocate resources
Input_param     :1)pointer to the inode structure
		:2)pointer to the file structure
output_param	:1)None
**************************************************************************/ 
static int driver_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
/***************************************************************************
Function Name   :driver_read
Description     :this function read data from the device and send it to the application
Input_param   	:1)pointer to the file structure
		:2)user space buffer
		:3)number of byte to be transferred to user space buffer
		:4)file offset
Output_param	:number of bytes reads successfully
****************************************************************************/ 
static ssize_t driver_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
      
        printk(KERN_INFO "Data Read successfully..\n");
        return 0;
}
/**************************************************************************
function Name 	:driver_write
Description 	:this function  accept data from user space application and write to device
Input_param	:1)pointer to file_structure
		:2)user space buffer which hold the rececived data from application		
		:3)len which is size of data
		:4)file offset 
Output_param	:number of bytes write successfully
***************************************************************************/
static ssize_t driver_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Data Write successfully..\n");
        return len;
}
/****************************************************************************
Function Name	:my_ioctl
Description     :this function used to implement the new command
input_param	:1)file descriptor of open file
		:2)ioctl command which is implemented to achieve the desired functionality	
		:3)arguments that needs to be passed to the ioctl command
output_param	:None
*****************************************************************************/ 
static long my_ioctl(struct file *file,unsigned int cmd,struct temp *arg)
{
	switch(cmd)
	{
	
		case WR_VALUE:
			copy_from_user(&val,(void*)arg,sizeof(struct temp));
			printk(KERN_INFO "value1 = %d\n",val.value1);
			printk(KERN_INFO "str = %s\n",val.str);
			printk(KERN_INFO "value2=%d\n",val.value2);
			break;
		case RD_VALUE:
			copy_to_user((void*)arg,&val,sizeof(struct temp));
			break;

	}
	return 0;

}

/*********************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
input param      : NONE
output param     : NONE
**********************************************************************************/
static int my_driver_init(void)
{
/**************************************************************************
Function Name	:alloc_chrdev_region
Description	:this function allocate major number
Input_param	:1)pointer to dev structure
		:2)first minor number
		:3)total count of numbers to be allocated
		:4)character device name
output_param	:None
**************************************************************************/       
	if((alloc_chrdev_region(&dev, 0, DEVICE_COUNT, "my_dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
/***************************************************************************
Function name	:cdev_init
Description	:initialize the cdev structure for creating cdev structure
input_param	:1)pointer to cdev structure
		:2)pointer to file opreation strucutre
ouput_param	:None
***************************************************************************/ 
        cdev_init(&my_cdev,&fops);
/*************************************************************************
Function Name	:cdev_add
Description 	:add the cdev to system
input_param	:1)pointer to cdev structure
		:2)first device number to which device respond
		:3)count is the number of device numbers that should be associated with device 
**************************************************************************/
         if((cdev_add(&my_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
/***************************************************************************
Function name 	:class_create
Description	:this function will create struct class for our device driver under /sys/class/
Input_param	:1)pointer to the module that is own this struct class
		:2)pointer to the string for the name of this class
Output_param	:struct class pointer 
****************************************************************************/ 
        if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 /******************************************************************************
Function Name	:device_create
Description	:creating the device and device created in sysfs


input_param	:1)pointer to the struct class that this device should be registerd
		:2)pointer to the parent struct device of this new device
		:3)dev_t for the char device to be added
		:4)void *drvdata for driver data		
		:5)string for the devices name
		
output		:None
*****************************************************************************/
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert successfully..!\n");
    return 0;
/*******************************************************************************
Function Name	:class_destroy
Description 	:destroying the created class from /sys/class
input_param	:1)pointer of class create function 
output_param	:1)None
********************************************************************************/ 
r_device:
        class_destroy(dev_class);
/*******************************************************************************
Function name	:unregister_chrdev_region
Description	:De-allocate or free device numbers
input_param	:1)first major number
		:2)total count of numbers to be de-allocated
Output_param	:None
*********************************************************************************/
r_class:
        unregister_chrdev_region(dev,DEVICE_COUNT);
        return -1;
}
/**********************************************************************************************
function	 : my_driver_exit
description	 : This function is invoked when module gets removed.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
void my_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, DEVICE_COUNT);
    	printk(KERN_INFO "Device Driver Remove successfully..!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);

































