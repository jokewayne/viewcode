#include <linux/init.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

#define MAXSTRLEN 255
#define DEV_NAME "char_chat"

struct cdev sink_chat_cdev;
static dev_t sink_chat_devNo;
struct class * sink_chat_class;
struct device *sink_chat_device;

struct semaphore sem;
wait_queue_head_t chat_que;
char buffer[MAXSTRLEN];
char *rd, *wr, *end;
int flag;

static ssize_t sink_chat_read(struct file *,char *,size_t ,loff_t *);
static ssize_t sink_chat_write(struct file *,const char *,size_t ,loff_t *);
static int sink_chat_open(struct inode *inode,struct file *filp);
static int sink_chat_release(struct inode *inode,struct file *filp);

struct file_operations sink_chat_fops =
{
        .read = sink_chat_read,
        .write = sink_chat_write,
        .open = sink_chat_open,
        .release = sink_chat_release,
};

static int sink_chat_init(void) {
    int result = 0;
    int err = 0;
    int i = 0;
    void *ptr_err;
    result = alloc_chrdev_region(&sink_chat_devNo, 0, 1, DEV_NAME);
    if (result < 0) {
            printk(KERN_ALERT "unable to allocate sink_chat device number");
            return result;
    }
    cdev_init(&sink_chat_cdev, &sink_chat_fops); 
    sink_chat_cdev.owner = THIS_MODULE;
    err = cdev_add(&sink_chat_cdev, sink_chat_devNo, 1);
    if (err ) {
        printk(KERN_INFO "Error %d adding sink_chat device", err);
        goto failed_cdev_add;
    } else {
        printk(KERN_INFO "register sink_chat to cdev success.");
        sema_init(&sem, 1 );
        init_waitqueue_head( &chat_que);
        rd = buffer;
        wr = buffer;
        end = buffer + MAXSTRLEN;
        for ( i = 0 ; i< MAXSTRLEN; i++) buffer[i] = '\0';
        flag = 0 ;
    }
    sink_chat_class = class_create(THIS_MODULE, DEV_NAME);
    ptr_err = sink_chat_class;
    if (IS_ERR(ptr_err)){
            printk(KERN_ALERT "class create failed, sink_chat_class");
            goto failed_class_create;
    }
    sink_chat_device = device_create(sink_chat_class, NULL, sink_chat_devNo, NULL, DEV_NAME);
    ptr_err = sink_chat_device;
    if (IS_ERR(ptr_err)) {
           printk(KERN_ALERT "device create failed, sink_chat_device");
           goto failed_device_create;
    }
    printk(KERN_INFO "sink_chat_device created.");
    return 0;
failed_device_create:
    class_destroy(sink_chat_class);
failed_class_create:
    cdev_del(&sink_chat_cdev);
    err = PTR_ERR(ptr_err);
    
failed_cdev_add:
    unregister_chrdev_region(sink_chat_devNo, 1);
    return err;
}



static int sink_chat_open(struct inode *inode,struct file *filp) {
    try_module_get(THIS_MODULE);
    printk(KERN_INFO " open sink_chat ");
    return 0;
}

static int sink_chat_release(struct inode *inode,struct file *filp) {
    module_put(THIS_MODULE);
    printk(KERN_INFO " release sink_chat ");
    return 0;
}

static void sink_chat_exit(void) {
    printk(KERN_INFO "sink_chat say Goodbye!\n");
    device_destroy(sink_chat_class, sink_chat_devNo);
    class_destroy(sink_chat_class);
    cdev_del(&sink_chat_cdev);
    unregister_chrdev_region(sink_chat_devNo, 1);
    printk(KERN_INFO "sink_chat leaves");
}

static ssize_t sink_chat_read(struct file *filp,char *buf,size_t len,loff_t *off)
{
    if ( wait_event_interruptible(chat_que, flag != 0 )) {
            return -ERESTARTSYS;
    }
    if (down_interruptible(&sem)) {
            return -ERESTARTSYS;
    }
    flag = 0;
    printk("into the read function\n");
    printk("the rd is %c, addr 0x%p\n",*rd,rd);
    if ( rd < wr) {
            len = min(len, (size_t)(wr - rd ));
    } else {
            len = min(len, (size_t)(end - rd));
    }
    printk("the len is %d\n",len);
    if (copy_to_user(buf, rd, len)) {
            printk(KERN_ALERT"copy failed\n");
            up(&sem);
            return -EFAULT;
    }
    printk("the read buffer is %s\n",buffer);
    rd = rd + len;
    if ( rd == end) rd = buffer;
    up(&sem);
    return len;

}

static ssize_t sink_chat_write(struct file *filp,const char *buf,size_t len,loff_t *off) {
    char *index;
    if (down_interruptible(&sem)) {
            return -ERESTARTSYS;
    }
    if ( rd <= wr ) {
            len = min( len, (size_t)(end - wr - 1 ));
    } else {
            len = min( len, (size_t)(rd - wr - 1));
    }
    printk("the write len is %d\n",len);
    if(copy_from_user(wr, buf,len)) {
            up(&sem);
            return -EFAULT;
    }
    printk("the write char %c, addr 0x%p\n",*wr,wr);
    index = buffer;
    while ( index < end && *index != '\0') {
        printk("the write buffer is %s\n",index);
        index += strlen(index)+1;
    }
    printk("the len of buffer is %d\n",strlen(buffer));
    wr = wr + len;
    *wr = '\0';
    wr++;
    if ( wr == end) wr = buffer;
    up(&sem);
    flag = 1;
    wake_up_interruptible(&chat_que);
    return len;
}


MODULE_LICENSE("GPL");
module_init(sink_chat_init);
module_exit(sink_chat_exit);
