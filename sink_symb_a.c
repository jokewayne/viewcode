#include <linux/init.h>
#include <linux/module.h>

static int add_sink(int a, int b) {
        return a+b;
}

static int multip_sink(int a, int b) {
        return a*b;
}

EXPORT_SYMBOL(add_sink);
EXPORT_SYMBOL(multip_sink);

