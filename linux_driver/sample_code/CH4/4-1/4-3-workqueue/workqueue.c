/* 
 * Copy from Terence
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

/* for k3 button */
int irq = EXYNOS4_GPX3(4);
struct work_struct workq;

void sam_workqueue(struct work_struct *work)
{
    pr_info("%s \r\n", __func__);
    msleep(1000);  /* sleep */
}

irqreturn_t workqueue_isr(int irq, void *dev_instance)
{
    pr_info("%s \r\n", __func__);
    schedule_work(&workq);
    return IRQ_NONE;
}

static int sam_workqueue_init(void) 
{
	int ret = 0;

    pr_info("workqueue driver installed.\n");
    INIT_WORK(&workq, sam_workqueue);

	irq = gpio_to_irq(irq);
    ret = request_irq(irq, workqueue_isr,
        IRQ_TYPE_EDGE_FALLING, "WorkQueue", NULL);
    if (ret) {
        pr_err("request_irq() failed (%d)\n", ret);
        goto out;
    }

out:
    return (ret);
} 

static void sam_workqueue_exit(void) 
{
	flush_scheduled_work();
    free_irq(irq, NULL);
    printk("sample driver removed.\n");
} 

module_init(sam_workqueue_init); 
module_exit(sam_workqueue_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is sam_workqueue  module");
MODULE_VERSION("V1");
