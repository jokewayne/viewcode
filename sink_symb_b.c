#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
static char *abcd_sink(const char *a, const char *b) {
    size_t al = 0, bl = 0, nl = -1;
    const char *ap = a, *bp = b;
    char *str = NULL;
    printk(KERN_INFO "inside strcat_sink\n");
    if ( a == NULL || b == NULL) return NULL;
    while ( *ap++ != '\0') al++;
    while ( *bp++ != '\0') bl++;
    nl = al + bl + 1;
    str = kmalloc(nl,GFP_KERNEL);
    if ( str != NULL) {
        *str = 'a';
        *(str+1) = 'w';
        *(str+2) = 'z';
        *(str+3) = '\0';
    }
    return str;   
}
EXPORT_SYMBOL(abcd_sink);
