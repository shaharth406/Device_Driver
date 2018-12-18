/************************************************************************************************
@fileName : hello_world.c
@author   : Arth Shah
@teamLead : Rajesh Dommaraju 
@details  : It is a simple HelloWorld driver to load a module in the kernel  
@license  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"Arth"
#define DRIVER_MODULE_DESC			"Simple Hello World Module"
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

/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/

static int hello_init(void);
static void hello_exit(void);


/**********************************************************************************************
function	 : hello_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/

static int hello_init(void) {
    printk(KERN_INFO "Module inserted Successfully..\n");
    printk(KERN_INFO "File : %s, function : %s, line : %d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**********************************************************************************************
function	 : hello_exit
description	 : This function is initialised when module gets removed.
		   This function is passed as an argument to module_exit.
input param      : NONE
output param     : NONE
**********************************************************************************************/

static void hello_exit(void) {
    printk(KERN_INFO "Module removed Successfully..\n"); 
    printk(KERN_INFO "File : %s, function : %s, line : %d\n", __FILE__, __func__, __LINE__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE (GPL_LICENSE);
MODULE_AUTHOR  (DRIVER_MODULE_AUTHOR);
MODULE_VERSION (DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION (DRIVER_MODULE_DESC);

