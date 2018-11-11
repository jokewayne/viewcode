#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>
//#include <linux/platform_device.h>
//platform_device 和 device区别在哪？
#include <linux/device.h>
enum IOCMD {
    SHOW = 100,
    TEST2 ,
    PARAM ,
    TEST4 ,
    MONDAY,
    TUESDAY ,
    SATURDAY ,
    SUNDAY 
};
#define SINKDEV_NAME "sinkDev"
static struct cdev sink_cdev;
static dev_t devno;
static struct class *sink_class;
static struct device *sink_dev;

static long sink_ioctl(struct file *filp, u_int cmd, u_long arg)
{
    int err = -EINVAL;
    switch (cmd) {
        case SHOW:
            printk(KERN_INFO "sink_ioctl show %ld\n",arg);
            break;
        case TEST2:
            printk(KERN_INFO "sink_ioctl test2\n");
            break;
        case PARAM:
            printk(KERN_INFO "sink_ioctl param:%ld\n", arg);
            break;
        case TEST4:
            printk(KERN_INFO "sink_ioctl test4\n");
            break;
        case MONDAY:
            printk(KERN_INFO "sink_ioctl MONDAY\n");
            break;
        case TUESDAY:
            printk(KERN_INFO "sink_ioctl TUESDAY\n");
            break;
        case SATURDAY:
            printk(KERN_INFO "sink_ioctl SATURDAY\n");
            break;
        case SUNDAY:
            printk(KERN_INFO "sink_ioctl SUNDAY\n");
            break;
        default:
           printk(KERN_INFO "sink_ioctl default [%d]\n", cmd);
            break;
    }
    return err;
}

static int sink_open(struct inode * inode, struct file * filp)
{
     printk(KERN_INFO "sink_open\n");
     return 0;
}

static int sink_release(struct inode * inode, struct file * filp)
{
     printk(KERN_INFO "sink_release\n");
     return 0;
}

/*文件操作结构体*/
static const struct file_operations sink_fops =
{
    .unlocked_ioctl	= sink_ioctl,
    .open = sink_open,
    .release = sink_release,
};


static int sink_init(void)
{
    int err;  
    void *ptr_err;
    printk(KERN_INFO "sink say hell world\n");
    /* 注册字符设备 */
    err = alloc_chrdev_region(&devno, 0, 1, SINKDEV_NAME);
    if (err != 0) {
		printk(KERN_ALERT "unable to allocate sink device number");
		goto failed_alloc_chrdev;
	}
    cdev_init(&sink_cdev,&sink_fops);
    sink_cdev.owner = THIS_MODULE;
    err = cdev_add(&sink_cdev,devno,1);
    if (err != 0) {
		printk(KERN_ALERT "unable to add sink device to /dev");
		goto failed_cdev_add;
	}
    /* Create sysfs entries */

	sink_class = class_create(THIS_MODULE, SINKDEV_NAME);
	ptr_err = sink_class;
	if (IS_ERR(ptr_err))
		goto failed_class_create;
    sink_dev = device_create(sink_class, NULL,
					devno, NULL,
					"sink");
	ptr_err = sink_dev;
	if (IS_ERR(ptr_err))
		goto failed_device_create;
    printk(KERN_INFO "sinkdev alloc\n");
    return 0;
failed_device_create:
    class_destroy(sink_class);
failed_class_create:
    cdev_del(&sink_cdev);
	err = PTR_ERR(ptr_err);
failed_cdev_add:
    unregister_chrdev_region(devno, 1);
failed_alloc_chrdev:
    return err;
}

static void sink_exit(void)
{
    printk(KERN_INFO "sink say Goodbye!\n");
    device_destroy(sink_class, devno);
	class_destroy(sink_class);
    cdev_del(&sink_cdev);
    unregister_chrdev_region(devno, 1); /*释放设备号*/
    printk(KERN_INFO "sink leaves!\n");
}

MODULE_LICENSE("GPL");

module_init(sink_init);
module_exit(sink_exit);

