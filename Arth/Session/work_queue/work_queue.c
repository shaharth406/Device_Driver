/*************************************************************************************************
@FileName	: intr.c
@Developer	: Arth shah
@TeamLead	: Rajesh Dommaraju 
@Description    : A Simple Interrupt with Work_queue  
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
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/workqueue.h>            // Required for workqueues
 
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
//Interrupt handler for IRQ 1. 
static irqreturn_t irq_handler(int irq,void *dev_id) {
        printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
        schedule_work(&workqueue);
        
        return IRQ_HANDLED;
}


void workqueue_fn(struct work_struct *work); 
 
/*Creating work by Static Method */
DECLARE_WORK(workqueue,workqueue_fn);
 
/*Workqueue Function*/
void workqueue_fn(struct work_struct *work)
{
        printk(KERN_INFO "Executing Workqueue Function\n");
}
 
/*************************************************************************************************
					GLOBAL VARIABLES		
*************************************************************************************************/  
 
volatile int etx_value = 0;
 
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

 /*************************************************************************************************
					LOCAL FUNCTIONS		
*************************************************************************************************/ 
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
 
/*************** Driver Fuctions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
 
 /************************************************************************************************
                      File Operations         
*************************************************************************************************/ 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .release        = etx_release,
};
 

 /*************************************************************************************************
Function        : etx_open()
Description     : Function used to open device file
Input param.    : instance of struct inode and struct file
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/ 
static int etx_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
 /*************************************************************************************************
Function        : etx_release()
Description     : Function used to release device file
Input param.    : instance of struct inode and struct file
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/ 
static int etx_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
 /*************************************************************************************************
Function        : etx_read()
Description     : Function used to read device file
Input param.    : instance of struct inode and struct file
				  intr_read(struct *inode, struct *file)
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/ 
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read function\n");
        asm("int $0x31");  // Corresponding to irq 1
        return 0;
}
/*************************************************************************************************
Function        : etx_write()
Description     : Function used to write device file
Input param.    : instance of struct inode and struct file
				  intr_write(struct *inode, struct *file)
Output param.   : 0 on success
				 -1 on failure
*************************************************************************************************/ 
static ssize_t etx_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write Function\n");
        return len;
}
 /*************************************************************************************************
Function        : etx_driver_init()
Description     : This function is initialised when module gets inserted.
		          This function is passed as an argument to module_init.
Input param.    : None
Output param.   : None
*************************************************************************************************/  
 
static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
 
        if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "etx_device", (void *)(irq_handler))) {
            printk(KERN_INFO "my_device: cannot register IRQ ");
                    goto irq;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
irq:
        free_irq(IRQ_NO,(void *)(irq_handler));
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&etx_cdev);
        return -1;
}
 /*************************************************************************************************
Function        : etx_driver_exit()
Description     : This function is initialised when module gets removed.
		          This function is passed as an argument to module_exit
Input param.    : None
Output param.   : None
*************************************************************************************************/ 
void __exit etx_driver_exit(void)
{
        free_irq(IRQ_NO,(void *)(irq_handler));
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com or admin@embetronicx.com>");
MODULE_DESCRIPTION("A simple device driver - Workqueue part 1");
MODULE_VERSION("1.10");
