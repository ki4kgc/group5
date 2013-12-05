/*  
 *  hello-2.c - Demonstrating the module_init() and module_exit() macros.
 *  This is preferred over using init_module() and cleanup_module().
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>           /* Required for file_operations */
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/wait.h>         /* Required for the wait queues */
#include <linux/sched.h>        /* Required for task states */
#include <asm/uaccess.h>

wait_queue_head_t my_queue;

struct device {
  char array[100];
  struct semaphore sem;
}char_arr;

static int device_open(struct inode *inode, struct file *file)
{
  printk(KERN_ALERT "Inside open \n");
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

/* Called when a process writes to dev file: echo "hi" > /dev/hello */
static ssize_t device_write(struct file *filp, const char *buff,
			    size_t len, loff_t * off)
{
  unsigned long ret;
  printk("Inside read \n");
  
  return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t len,
			   loff_t * offset)
{
  unsigned long ret;
  int condition = 1; 

  printk(KERN_INFO "Inside read");	
  /* While condition is 0, enter wait queue */
  wait_event_interruptible(my_queue,condition);
  
  ret = copy_to_user(buffer, char_arr.array, len);
  return ret;
}

struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release

};

struct cdev *kernel_cdev;

static int __init hello_2_init(void)
{
	 printk(KERN_INFO "Starting to break...\n");

	/*Making stuff wait*/
	
	 int ret, Major;
         dev_t dev_no, dev;

	 kernel_cdev = cdev_alloc();
	 kernel_cdev->ops = &fops;
         kernel_cdev->owner = THIS_MODULE;
         
         ret = alloc_chrdev_region(&dev_no, 0, 1, "chr_arr_dev");
	 
	 /* Makes sure major number allocation is successful */
	 if (ret < 0) {
	   printk(KERN_INFO "Major number allocation failed \n");
	   return ret;
	 }
	 
	 Major = MAJOR(dev_no);
	 dev = MKDEV(Major, 0);
	 sema_init(&char_arr.sem, 1);
	 printk(KERN_INFO "The major number for your device is %d\n", Major);
	 ret = cdev_add(kernel_cdev, dev, 1);

	 if (ret <0) {
	   printk(KERN_INFO "Unable to allocate cdev");
	   return ret;
	 }
	 
   
	init_waitqueue_head(&my_queue);
	

	
	printk(KERN_INFO "Hello world 2 TEST.\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

static void __exit hello_2_exit(void)
{
	printk(KERN_INFO "Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

