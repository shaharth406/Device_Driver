/************************************************************************************************
@fileName : device_file.c
@author   : Arth Shah
@teamLead : Rajesh Dommaraju 
@details  : It is a simple devie file which one created  automatic using class structure  
@license  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"ARTH"
#define DRIVER_MODULE_DESC			"Device file creation using class structure"
#define DRIVER_MODULE_VERSION			"V1.0"
	
/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/



/******************************************************************************
			LOCAL VARIABLES         
*******************************************************************************/
dev_t dev = 0;
static struct class *dev_class;
/*******************************************************************************
                        LOCAL FUNCTIONS         
*******************************************************************************/

static int hello_init(void);
static void hello_exit(void);


/**********************************************************************************************
function         : hello_init
description      : This function is initialised when module gets inserted.
                   It also create class structure and device file 
input param      : NONE
output param     : NONE
**********************************************************************************************/

static int hello_init(void)
{
        if((alloc_chrdev_region(&dev,0,1,"arth_dev"))<0)
	{
		printk(KERN_INFO "Cannot allocate major number for device\n");
		return -1;
	}	
        printk(KERN_INFO "Major=%d Minor=%d",MAJOR(dev),MINOR(dev));
       
	if((dev_class=class_create(THIS_MODULE,"my_class"))==NULL)
	{
		printk(KERN_INFO "cannot create struct class for this device\n");
		goto r_class;	
	}
	if((device_create(dev_class,NULL,dev,NULL,"arth_device"))==NULL)
	{
		printk(KERN_INFO "cannot create the device\n");
		goto r_device;
	}
	 printk(KERN_INFO "kernal module inserted successfully..\n");
        return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

/**********************************************************************************************
function         : hello_exit
description      : This function is initialised when module gets removed.
                   It also unregister device.
input param      : NONE
output param     : NONE
**********************************************************************************************/

static void  hello_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
        unregister_chrdev_region(dev,1);
        printk(KERN_INFO "Kernal Module Removed Successfully..\n");
       // return 0;
                                                                                    
}
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE (GPL_LICENSE);
MODULE_AUTHOR  (DRIVER_MODULE_AUTHOR);
MODULE_VERSION (DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION (DRIVER_MODULE_DESC);





