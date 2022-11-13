#include <linux/init.h>
#include <linux/module.h>

void *ptr;
int order;

void *ptr2;
int order2;

static int buddy_init(void)
{
    printk("Buddy driver installed...\n");

    order = 4;

    ptr = (void *)__get_free_pages(GFP_KERNEL, order);
    if (ptr)
        printk("2^%d * %ld bytes allocated\n", order, PAGE_SIZE);
    
    order2 = get_order(PAGE_SIZE * 4);
    ptr2 = (void *)__get_free_pages(GFP_KERNEL, order2);
    if (ptr2)
        printk("2^%d * %ld bytes allocated\n", order2, PAGE_SIZE);

    return 0;
}

static void buddy_exit(void)
{
    printk("buddy driver removed...\n");
    printk("buddy driver removed order  %d ...\n", order);
    printk("buddy driver removed order2  %d ...\n", order2);

	free_pages((unsigned long)ptr, order);
	free_pages((unsigned long)ptr2, order2);
}

module_init(buddy_init);
module_exit(buddy_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is buddy  module");
MODULE_VERSION("V1");
