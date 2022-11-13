#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

void __iomem *reg;
static int mmio_init(void)
{
    printk("mmio driver installed.\n");

    reg = ioremap(0x10000000, 4);
    if (reg) {
        printk("addr  = [%lu]\n", (unsigned long int)reg);
        printk("value = [%x]\n", readl(reg));
    }

    return 0;
}

static void mmio_exit(void)
{
    iounmap(reg);
    printk("mmio driver removed.\n");
}

module_init(mmio_init);
module_exit(mmio_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
