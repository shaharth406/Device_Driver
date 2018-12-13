/************************************************************************************************
@ileName : module1.c 
@author	 : Arth Shah
@teamLead: Rajesh Dommaraju 
@etails  : It is a driver for making a function, variables available for others to use.
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"SPANIDEA"
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
int var=100;

EXPORT_SYMBOL(var);


/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int my_init(void);
void my_exit(void);
void print_hello(int num);

/**********************************************************************************************
function	 : print_hello(int num)
description	 : This function will print "Hello" num of time when called by other module.
input param      : int num
output param     : NONE
**********************************************************************************************/
void print_hello(int num)
{
	while(num--)
	{
		printk(KERN_INFO "Hello\n");
	}

}

EXPORT_SYMBOL(print_hello);

/**********************************************************************************************
function	 : my_init
description	 : This function is initialised when module gets inserted.
		    
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int my_init(void)
{

printk(KERN_INFO "Hello from module1 \n");
return 0;
}
/**********************************************************************************************
function	 : my_exit
description	 : This function is initialised when module gets removed.
input param      : NONE
output param     : NONE
**********************************************************************************************/
void my_exit(void)
{
printk(KERN_INFO "exit from module1\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);
