#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

extern wait_queue_head_t sleep_wait; // Step 1
extern bool flag;

static int wakeupone_init(void)
{
        wait_queue_head_t *wq = &sleep_wait; // Step 2

        printk("[%s] - driver loaded\n", __func__);

        flag = 1;

        wake_up(wq);

        return 0;
}

static void wakeupone_exit(void)
{
        printk(KERN_ALERT "driver unloaded\n");
}

module_init(wakeupone_init);
module_exit(wakeupone_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is read/write test  module");
MODULE_VERSION("V1");
