/*************************************************************************************************
@FileName	: intr.c
@Developer	: Arth shah
@TeamLead	: Rajesh Dommaraju 
@Description    : A Simple IRQ Handler  
@License	: SpanIdea Systems Pvt. Ltd. All rights reserved.
*************************************************************************************************/

/*************************************************************************************************
					INCLUDES
*************************************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user() 
#include <linux/interrupt.h>
#include <asm/io.h>
 
/************************************************************************************************* 
					LOCAL MACROS           
*************************************************************************************************/ 
#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"Arth Shah"
#define DRIVER_MODULE_DESC	"A Simple Interrupt module"
#define DRIVER_MODULE_VERSION	"V1.0"
#define IRQ_NO			1
 
/*************************************************************************************************
					Interrupt handler for IRQ 1	
*************************************************************************************************/  
 
static irqreturn_t irq_handler(int irq,void *dev_id) {
  printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
  return IRQ_HANDLED;
}
 
/*************************************************************************************************
					GLOBAL VARIABLES		
*************************************************************************************************/ 
volatile int intr_value = 0; 
static dev_t dev = 0;
static struct class *dev_class;
static struct cdev intr_cdev;

/*************************************************************************************************
					LOCAL FUNCTIONS		
*************************************************************************************************/  
static int  intr_driver_init(void);
static void  intr_driver_exit(void);
 
static int intr_open(struct inode *inode, struct file *file);
static int intr_release(struct inode *inode, struct file *file);
static ssize_t intr_read(struct file *filp,   char __user *buf, size_t len,loff_t * off);
static ssize_t intr_write(struct file *filp,const  char *buf, size_t len, loff_t * off);
 
/************************************************************************************************
                      File Operations         
*************************************************************************************************/  
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = intr_read,
        .write          = intr_write,
        .open           = intr_open,
        .release        = intr_release,
};

/*************************************************************************************************
Function        : intr_open()
Description     : Function used to open device file
Input param.    : instance of struct inode and struct file
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/  
static int intr_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

/*************************************************************************************************
Function        : intr_release()
Description     : Function used to release device file
Input param.    : instance of struct inode and struct file
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/  
static int intr_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

/*************************************************************************************************
Function        : intr_read()
Description     : Function used to read device file
Input param.    : instance of struct inode and struct file
				  intr_read(struct *inode, struct *file)
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/  
static ssize_t intr_read(struct file *filp,  char  *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read function\n");
        asm("int $0x31");  // Corresponding to irq 1
        return 0;
}

/*************************************************************************************************
Function        : intr_write()
Description     : Function used to write device file
Input param.    : instance of struct inode and struct file
				  intr_write(struct *inode, struct *file)
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/ 
static ssize_t intr_write(struct file *filp,const char  *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write Function\n");
        return len;
}
 
/*************************************************************************************************
Function        : intr_driver_init()
Description     : This function is initialised when module gets inserted.
		          This function is passed as an argument to module_init.
Input param.    : None
Output param.   : None
*************************************************************************************************/  
static int  intr_driver_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 1, "intr_Dev")) <0){         /*Allocating Major number*/
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
        cdev_init(&intr_cdev,&fops);                                  /*Creating cdev structure*/

        if((cdev_add(&intr_cdev,dev,1)) < 0){           /*Adding character device to the system*/
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
        if((dev_class = class_create(THIS_MODULE,"intr_class")) == NULL){ /*Creating struct class*/
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
        if((device_create(dev_class,NULL,dev,NULL,"intr_device")) == NULL){  /*Creating device*/
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
 
        if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "intr_device",(void*)irq_handler)) {
            printk(KERN_INFO "my_device: cannot register IRQ ");
                    goto irq;
        }
        printk(KERN_INFO "Device Driver Inserted \n");
    return 0;
 
irq:
        free_irq(IRQ_NO,(void*)irq_handler);

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&intr_cdev);
        return -1;
}

/*************************************************************************************************
Function        : intr_driver_exit()
Description     : This function is initialised when module gets removed.
		          This function is passed as an argument to module_exit
Input param.    : None
Output param.   : None
*************************************************************************************************/  
static void intr_driver_exit(void)
{
        free_irq(IRQ_NO,(void*)irq_handler);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&intr_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Removed \n");
}
 
module_init(intr_driver_init);
module_exit(intr_driver_exit);
 
MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);
MODULE_VERSION(DRIVER_MODULE_VERSION);
