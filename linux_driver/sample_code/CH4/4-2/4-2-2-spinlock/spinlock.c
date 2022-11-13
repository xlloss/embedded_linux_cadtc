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
#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h> 
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/hardirq.h>

#define DRIVER_NAME "spinlock_test"
#define USE_SPINLOCK

static unsigned int sema_test_major = 0;
static unsigned int num_of_dev = 1;
struct completion comp;
unsigned  enter_count = 0;
struct spinlock_test_data {
    struct cdev spinlock_test_cdev;
    struct class *spinlock_test_class;
    struct device *spinlock_test_device;
    spinlock_t spinlock;
    char val;
} spinlock_test_data;

ssize_t spinlock_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    int retval, i = 5;
    char val;

    enter_count++;

    if (copy_from_user(&val, buf, count)) {
        retval = -EFAULT;
        printk("Write Error...\n");
        goto out;
    }
    retval = count;

    printk("spinlock_test_write wait ... ready\n");

#ifdef USE_SPINLOCK
    spin_lock(&spinlock_test_data.spinlock);
#endif

    printk("+++ spinlock_test_write +++ get\n");
    while (i > 0) {
        spinlock_test_data.val = val;
        printk("+++ spinlock_test_write val %d\n", spinlock_test_data.val);
        udelay(100);
        i--;
    };

#ifdef USE_SPINLOCK
    spin_unlock(&spinlock_test_data.spinlock);
#endif

    printk("--- spinlock_test_write --- release\r\n");
out:
    return retval;
}

ssize_t spinlock_test_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int retval;
	char val;


	printk("spinlock_test_read wait ...\r\n");

    printk("+++ spinlock_test_read +++ => ready\r\n");
#ifdef USE_SPINLOCK
    spin_lock(&spinlock_test_data.spinlock);
#endif
    printk("+++ spinlock_test_read +++ => get\r\n");

	val = spinlock_test_data.val;
    printk("+++ spinlock_test_read val %d\n", spinlock_test_data.val);

#ifdef USE_SPINLOCK
    spin_unlock(&spinlock_test_data.spinlock);
#endif
    printk("--- spinlock_test_read --- release\r\n");
    if (copy_to_user(buf, &val, count)) {
        retval = -EFAULT;
        printk("Read Error...\n");
                goto out;
    }

    retval = count;
out:
    return (retval);
}

static int spinlock_test_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "Call sema_test_open\n");	
	return 0;
}

static int spinlock_test_close(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Call sema_test_close\n");
    return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
    .open = spinlock_test_open,
    .release = spinlock_test_close,
	.read = spinlock_test_read,
	.write = spinlock_test_write,
};

static int spinlock_test_init(void) 
{ 
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t devt;

    alloc_ret = alloc_chrdev_region(&devt, 0, num_of_dev, DRIVER_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&spinlock_test_data.spinlock_test_cdev, &fops);
    cdev_ret = cdev_add(&spinlock_test_data.spinlock_test_cdev,devt, num_of_dev);
   	if(cdev_ret)
        goto error;

    spinlock_test_data.spinlock_test_class = class_create(THIS_MODULE, DRIVER_NAME);
    spinlock_test_data.spinlock_test_device = device_create(spinlock_test_data.spinlock_test_class,
        NULL, devt, NULL, "%s%d", DRIVER_NAME, 0);

    /* init spinlock */
    spin_lock_init(&spinlock_test_data.spinlock);

    sema_test_major = MAJOR(devt);
    printk(KERN_ALERT "%s driver(major %d) installed.\n", DRIVER_NAME, sema_test_major);

#ifdef USE_SPINLOCK
    pr_info("USE_SPINLOCK Enable\r\n");
#endif

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&spinlock_test_data.spinlock_test_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(devt, num_of_dev);

    return -1;
} 

static void spinlock_test_exit(void) 
{
    dev_t devt = MKDEV(sema_test_major, 0);

    cdev_del(&spinlock_test_data.spinlock_test_cdev);
    device_del(spinlock_test_data.spinlock_test_device);
    class_destroy(spinlock_test_data.spinlock_test_class);
    unregister_chrdev_region(devt, num_of_dev);
    printk(KERN_ALERT"%s driver removed\n", DRIVER_NAME); 
} 

module_init(spinlock_test_init); 
module_exit(spinlock_test_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is alloc_chrdev_region  module");
MODULE_VERSION("V1");
