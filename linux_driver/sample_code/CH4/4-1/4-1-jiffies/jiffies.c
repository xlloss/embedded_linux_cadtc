#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

static int jiffies_test_init(void)
{
	struct timeval time_val;
	struct timespec time_spec;
	unsigned long before, after;

	printk("HZ = [%d]\n", HZ);
	before = (unsigned long)jiffies;
    printk("Before jiffies = [%ld]\n", before);

	while (time_before(jiffies, before + (HZ * 2)))
	//while (time_before(jiffies, before + msecs_to_jiffies(1000)))
	//msleep(1000);
	//mdelay(1000);

	after = (unsigned long)jiffies;
	printk("After jiffies  = [%ld]\n", after);

	jiffies_to_timeval(after - before, &time_val);
	jiffies_to_timespec(after - before, &time_spec);

	printk("Elapsed time = [%lu]sec\n", (after - before) / HZ);
	printk("Elapsed time = [%d]msec\n", jiffies_to_msecs (after - before));
	printk("Elapsed time = [%lu]sec [%lu]usec\n", time_val.tv_sec, time_val.tv_usec);
	printk("Elapsed time = [%lu]sec [%lu]nsec\n", time_spec.tv_sec, time_spec.tv_nsec);

	return 0;
}

static void jiffies_test_exit(void)
{
	printk("%s driver removed...\n", __func__);
}

module_init(jiffies_test_init);
module_exit(jiffies_test_exit);

MODULE_LICENSE("GPL");
