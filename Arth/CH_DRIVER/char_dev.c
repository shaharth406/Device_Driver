/************************************************************************************************
@fileName : char_dev.c
@author   : Arth Shah
@teamLead : Rajesh Dommaraju 
@details  : It is a simple char driver with Major and Minor no to load a module in the kernel  
@license  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"ARTH"
#define DRIVER_MODULE_DESC			"Simple Major Minor No with Static Allocation"
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
dev_t dev = MKDEV(235,0);
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/

static int hello_init(void);
static void hello_exit(void);


/**********************************************************************************************
function	 : hello_init
description	 : This function is initialised when module gets inserted.
		   It also register device in char. device with 235 Major no
input param      : NONE
output param     : NONE
**********************************************************************************************/

static int hello_init(void)
{
	register_chrdev_region(dev,0,"My_dev");
	printk(KERN_INFO "Major=%d Minor=%d",MAJOR(dev),MINOR(dev));
	printk(KERN_INFO "kernal module inserted successfully..\n");
	return 0;
}

/**********************************************************************************************
function	 : hello_exit
description	 : This function is initialised when module gets removed.
		   It also unregister device in char. device with 235 Major no
input param      : NONE
output param     : NONE
**********************************************************************************************/

static void  hello_exit(void)
{

	unregister_chrdev_region(dev,0);
	printk(KERN_INFO "Kernal Module Removed Successfully..\n");
	

}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE (GPL_LICENSE);
MODULE_AUTHOR  (DRIVER_MODULE_AUTHOR);
MODULE_VERSION (DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION (DRIVER_MODULE_DESC);





