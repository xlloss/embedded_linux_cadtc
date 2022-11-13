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
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>

#define DRIVER_NAME "waitevent"

static unsigned int waitevent_major = 0;
static unsigned int num_of_dev = 1;
static struct cdev waitevent_cdev;

static DECLARE_WAIT_QUEUE_HEAD(wq); // step 1
static bool flag = 0;

ssize_t waitevent_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("[%s] process %i (%s) going to sleep\n", __func__,
        current->pid, current->comm);

    wait_event_interruptible(wq, flag); // step 2
	//wait_event_interruptible_exclusive(wq, flag != 0);
    //wait_event_timeout(wq, flag != 0, 10 * HZ);

    printk("[%s] awoken %i (%s)\n", __func__, current->pid, current->comm);
    return 0;
}

ssize_t waitevent_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
    printk("[%s] process %i (%s) awakening the readers...\n",
        __func__, current->pid, current->comm);

    flag = 1;
    wake_up(&wq); // step 3
    //wake_up_interruptible(&wq); // step 3
	//wake_up_interruptible_all(&wq);
	flag = 0;

    return count;
}

static int waitevent_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Call waitevent_open...\n");
    return 0;
}

static int waitevent_close(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Call waitevent_close...\n");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = waitevent_open,
    .release = waitevent_close,
    .read = waitevent_read,
    .write = waitevent_write,
};

static int waitevent_init(void)
{
    int alloc_ret = 0;
    int cdev_ret = 0;

    dev_t dev;
    alloc_ret = alloc_chrdev_region(&dev, 0, num_of_dev, DRIVER_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&waitevent_cdev, &fops);
    cdev_ret = cdev_add(&waitevent_cdev, dev, num_of_dev);
   	if (cdev_ret)
        goto error;

    waitevent_major = MAJOR(dev);
    pr_info("%s driver(major %d) installed.\n",
        DRIVER_NAME, waitevent_major);

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&waitevent_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(dev,num_of_dev);

    return -1;
}

static void waitevent_exit(void)
{
    dev_t dev = MKDEV(waitevent_major, 0);

    cdev_del(&waitevent_cdev);
    unregister_chrdev_region(dev,num_of_dev);
    printk(KERN_ALERT"%s driver removed\n",DRIVER_NAME);
}

module_init(waitevent_init);
module_exit(waitevent_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is waitevent test module");
MODULE_VERSION("V1");
