/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/wait.h>		/* Needed to for wait queue*/

int init_module(void)
{	
	printk(KERN_INFO "Starting to break...\n");

	/*Making stuff wait*/
	
	wait_queue_head_t my_queue;
	init_waitqueue_head(&my_queue);
	int condition = 0;
	
	/*While condition is 0, enter wait queue*/
	wait_event_interruptible(my_queue,condition);
	
	printk(KERN_INFO "Hello world 1 TEST.\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
