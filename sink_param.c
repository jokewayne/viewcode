#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int myvar = 666;

module_param(myvar, int , S_IRUGO);
//MODULE_PARAM_DESC(myvar, "this is my var, just a test" "second line description.");

static int __init sink_param_init(void) {
        printk(KERN_INFO "hello I am sink_param\n");
        printk(KERN_INFO " my var is %d\n",myvar);
        return 0;
}

static void __exit sink_param_exit(void) {
        printk(KERN_INFO "good bye, from sink_param\n");
}

module_init(sink_param_init);
module_exit(sink_param_exit);


