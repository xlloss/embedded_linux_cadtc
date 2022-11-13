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
#include <linux/delay.h>

//#define TEST
#undef PDEBUG
//#undef HELLO_DEBUG
#define HELLO_DEBUG

#define HELLO_DEBUG 0

#if HELLO_DEBUG

#endif

#ifdef HELLO_DEBUG

#endif


#ifdef HELLO_DEBUG
	#ifdef __KERNEL__
		#error 1 DEFINE HELLO_DEBUG
		#define PDEBUG(fmt, args...) printk(KERN_DEBUG "PDEBUG : " fmt, ## args)
	#else
		#error 2 DEFINE HELLO_DEBUG
	#define PDEBUG(fmt, args...) fprintf(stderr,fmt, ## args)
	#endif
#else
	//#error NO_DEFINE HELLO_DEBUG
	#error 3 DEFINE HELLO_DEBUG
	#define PDEBUG(fmt,args...)
#endif

static int hello_init(void)
{
#ifdef TEST
	int test = 0;
#endif
	int i = 0;
	pr_err("TESME!!!!!");
	printk(KERN_CRIT"This File is [%s]\n",__FILE__);
	printk(KERN_NOTICE"In Function [%s]\n",__func__);
	printk(KERN_NOTICE"At Line [%d]\n",__LINE__);

	printk(KERN_NOTICE "hello_init...\n");

	PDEBUG("Debug message here\n");
#ifdef TEST
	printk(KERN_INFO "test %d\r\n", test);
#endif


	for(i = 0; i < 20; i++) {
		if (printk_ratelimit())
			printk(KERN_ALERT "Number of times [%d]\n", i);

		if (i == 3)
			msleep(1000);
	}

	return 0;
}

static void hello_exit(void)
{
	printk("Call %s\n",__func__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");
