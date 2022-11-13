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
#include <linux/uaccess.h>
#include <linux/sched.h>

#define DRIVER_NAME "sleepone"

static unsigned int sleepone_major = 0;
static unsigned int num_of_dev = 1;
static struct cdev sleepone_cdev;

wait_queue_head_t sleep_wait; // Step 1
EXPORT_SYMBOL(sleep_wait);
bool flag;
EXPORT_SYMBOL(flag);

ssize_t sleepone_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("Call function [%s]\n",__func__);
    int i;
    unsigned char val = 'A';
    int retval;

    //wait_event_timeout(sleep_wait, flag != 0, 10 * HZ);
    wait_event(sleep_wait, flag != 0);

    printk("[%s] - Start read data!\n", __func__);

    if (copy_to_user(buf, &val, 1)) {
        retval = -EFAULT;
        goto out;
    }

    retval = count;
out:
    return (retval);

}

static int sleepone_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "Call sleepone_open\n");
	return 0;
}

static int sleepone_close(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Call sleepone_close\n");
    return 0;
}


struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = sleepone_open,
    .release = sleepone_close,
    .read = sleepone_read,
};

static int sleepone_init(void) 
{ 
    dev_t dev;
    int alloc_ret = 0;
    int cdev_ret = 0;

    alloc_ret = alloc_chrdev_region(&dev, 0, num_of_dev, DRIVER_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&sleepone_cdev, &fops);
    cdev_ret = cdev_add(&sleepone_cdev,dev, num_of_dev);
 
   	if (cdev_ret)
        goto error;

    init_waitqueue_head(&sleep_wait);
    flag = 0;

    sleepone_major = MAJOR(dev);
    printk(KERN_ALERT"%s driver(major %d) installed.\n",
        DRIVER_NAME,sleepone_major);

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&sleepone_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(dev,num_of_dev);

    return -1;
} 

static void sleepone_exit(void) 
{
    dev_t dev = MKDEV(sleepone_major, 0);

    cdev_del(&sleepone_cdev);
    unregister_chrdev_region(dev,num_of_dev);
    printk(KERN_ALERT"%s driver removed\n",DRIVER_NAME); 
} 

module_init(sleepone_init); 
module_exit(sleepone_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is sleepone test  module");
MODULE_VERSION("V1");
