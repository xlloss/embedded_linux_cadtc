#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

void *memptr1, *memptr2;

static int kmalloc_init(void)
{
    printk("kmalloc driver installed.\n");

    memptr1 = kzalloc(8193, GFP_KERNEL);
    printk("Kmalloc %p\n", memptr1);

	memptr2 = vzalloc(4096);
    printk("Vmalloc %p\n", memptr2);

    return 0;
}

static void kmalloc_exit(void)
{
    printk("kmalloc driver removed.\n");
    kfree(memptr1);
    vfree(memptr2);
}

module_init(kmalloc_init);
module_exit(kmalloc_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is kmalloc module");
MODULE_VERSION("V1");
