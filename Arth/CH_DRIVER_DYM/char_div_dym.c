/************************************************************************************************
@fileName : char_dev_dym.c
@author   : Arth Shah
@teamLead : Rajesh Dommaraju 
@details  : It is a simple char driver with Major and Minor no to load a module in the kernel  
@license  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/fs.h>
 
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"ARTH"
#define DRIVER_MODULE_DESC			"Simple Major Minor No with Dynamic Allocation"
#define DRIVER_MODULE_VERSION			"V1.0"

/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/
dev_t dev = 0;
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/

static int hello_world_init(void);
void  hello_world_exit(void);

/**********************************************************************************************
function	 : hello_world_init
description	 : This function is initialised when module gets inserted.
		   It also register device in char. device with dynamically
input param      : NONE
output param     : NONE
**********************************************************************************************/
 
static int hello_world_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "my_Dev_dym")) <0){
                printk(KERN_INFO "Cannot allocate major number for device 1\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	return 0;
}
 
/**********************************************************************************************
function	 : hello_world_exit
description	 : This function is initialised when module gets removed.
		   It also unregister device in char. device with dynamically
input param      : NONE
output param     : NONE
**********************************************************************************************/
void  hello_world_exit(void)
{
        unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE (GPL_LICENSE);
MODULE_AUTHOR  (DRIVER_MODULE_AUTHOR);
MODULE_VERSION (DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION (DRIVER_MODULE_DESC);

