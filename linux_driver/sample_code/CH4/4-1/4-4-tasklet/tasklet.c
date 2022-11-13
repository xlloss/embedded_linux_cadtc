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
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

/* for k4 button */
int irq = EXYNOS4_GPX3(5);

struct tasklet_struct tasklet;

void t_tasklet(unsigned long data)
{
    /* task cannot goto sleep */
    msleep(1000);
    pr_info("%s\r\n", __func__);
}

irqreturn_t tasklet_isr(int irq, void *dev_instance)
{
    tasklet_schedule(&tasklet);
    return IRQ_NONE;
}

static int sample_tasklet_init(void) 
{
    int ret;

    printk("Tasklet driver loaded\n");
    tasklet_init(&tasklet, t_tasklet, 0);

    irq = gpio_to_irq(irq);
    ret = request_irq(irq, tasklet_isr,
        IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "Tasklet", NULL);
    if (ret) {
        printk("request_irq() failed (%d)\n", ret);
        tasklet_kill(&tasklet);
        goto out;
    }

out:
    return (ret);
}

static void sample_tasklet_exit(void) 
{
	printk("Tasklet driver removed.\n");

    tasklet_kill(&tasklet);
    free_irq(irq, NULL);
}

module_init(sample_tasklet_init); 
module_exit(sample_tasklet_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is tasklet  module");
MODULE_VERSION("V1");
