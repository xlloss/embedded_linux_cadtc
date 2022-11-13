#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init_V3(void)
{
	printk(KERN_EMERG "Hello world V3!\n");
    pr_info("Hello world V3!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("Goodbye, Hello world V2!\n");
}

module_init(hello_init_V3);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is hello module");
MODULE_VERSION("V2");
