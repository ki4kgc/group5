#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // required for various structures related to files liked fops.
#include <asm/uaccess.h> // required for copy_from and copy_to user functions
#include <linux/sem.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/wait.h> // Required for the wait queues
#include <linux/sched.h> // Required for task states (TASK_INTERRUPTIBLE etc )


static int Major;
struct proc_dir_entry *Our_Proc_File;
wait_queue_head_t queue;
char flag = 'n';


struct device {
    char array[100];
    struct semaphore sem;
}char_arr;


int open(struct inode *inode, struct file *filp)
{
   
    printk(KERN_INFO "Inside open \n");
    /*if(down_interruptible(&char_arr.sem)) {
        printk(KERN_INFO " could not hold semaphore");
        return -1;
    }*/

	try_module_get(THIS_MODULE);
    return 0;
}


int release(struct inode *inode, struct file *filp) {
    printk (KERN_INFO "Inside close \n");
    printk(KERN_INFO "Releasing semaphore");
    up(&char_arr.sem);
    return 0;
}


ssize_t read(struct file *filp, char *buff, size_t count, loff_t *offp) {
    unsigned long ret;
    printk("Inside read \n");
    wait_event_interruptible(queue, flag != 'n');
    printk(KERN_INFO "Woken Up");
    ret = copy_to_user(buff, char_arr.array, count);
    return ret;
}


ssize_t write(struct file *filp, const char *buff, size_t count, loff_t *offp) {   
    unsigned long ret;
    printk(KERN_INFO "Inside write \n");
    ret =    copy_from_user(char_arr.array, buff, count);
    return ret;
}






int write_proc(struct file *file,const char *buffer,unsigned long count,void *data)
{
int ret=0;
printk(KERN_INFO "procfile_write /proc/wait called");
ret = __get_user(flag,buffer);
printk(KERN_INFO "%c",flag);
wake_up_interruptible(&queue);
return ret;
}


void create_new_proc_entry()
{
Our_Proc_File = create_proc_entry("wait", 0666, NULL);
   
    if(Our_Proc_File == NULL) {
        remove_proc_entry("wait", NULL);
        printk(KERN_ALERT "Error: could not initialize /proc/wait\n");
        return -ENOMEM;
    }
    Our_Proc_File->write_proc = write_proc;
    printk(KERN_INFO "/proc/wait created \n");
   
}


struct file_operations fops = {
    read:        read,
    write:        write,
    open:         open,
    release:    release
};




struct cdev *kernel_cdev;




int char_arr_init (void) {
    int ret;
    dev_t dev_no,dev;


    kernel_cdev = cdev_alloc();   
     kernel_cdev->ops = &fops;
    kernel_cdev->owner = THIS_MODULE;
    printk (" Inside init module\n");
     ret = alloc_chrdev_region( &dev_no , 0, 1,"chr_arr_dev");
    if (ret < 0) {
        printk("Major number allocation is failed\n");
        return ret;   
    }
   
    Major = MAJOR(dev_no);
    dev = MKDEV(Major,0);
    sema_init(&char_arr.sem,1);   
    printk (" The major number for your device is %d\n", Major);
    ret = cdev_add( kernel_cdev,dev,1);
    if(ret < 0 )
    {
    printk(KERN_INFO "Unable to allocate cdev");
    return ret;
    }
    init_waitqueue_head(&queue);
    create_new_proc_entry();
    return 0;
}


void char_arr_cleanup(void) {
    printk(KERN_INFO " Inside cleanup_module\n");
    remove_proc_entry("wait",NULL);
    cdev_del(kernel_cdev);
    unregister_chrdev_region(Major, 1);
}
MODULE_LICENSE("GPL");   
module_init(char_arr_init);
module_exit(char_arr_cleanup);
