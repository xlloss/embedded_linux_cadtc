/*
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
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/mm.h>

#define TIMEOUT_VALUE (3 * HZ)

static struct timer_list timerfn;
unsigned int *my_data;

static void timer_timeout(unsigned long data)
{
	unsigned int my_data = (unsigned int)data;

	pr_info("%s my_data = %d\n", __func__, my_data);
	/*
	 * can not go to sleep
	 */

	/* msleep(1000); */
}

static int timer_init(void)
{
	my_data = kmalloc(sizeof(unsigned int) * 1, GFP_KERNEL);
	pr_info("%s driver loaded...\n", __func__);

	*my_data = 10;
	timerfn.expires = jiffies + TIMEOUT_VALUE;
	timerfn.function = timer_timeout;
	timerfn.data = *my_data;

	add_timer(&timerfn);

	return 0;
}

static void timer_exit(void)
{
	int ret;

	pr_info("%s driver removed...\n", __func__);
	ret = del_timer_sync(&timerfn);
	kfree(my_data);
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
