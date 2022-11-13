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
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/hardirq.h>

#define DRIVER_NAME "sema_test"
#define USE_SEMA

static unsigned int sema_test_major = 0;
static unsigned int num_of_dev = 1;


struct sema_test_data {
    struct cdev sema_test_cdev;
    struct class *sema_test_class;
    struct device *sema_test_device;
    struct semaphore sem;
    char val;
} sema_test_data;

ssize_t sema_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    int retval;
    char val;

    if (copy_from_user(&val, buf, count)) {
        retval = -EFAULT;
        printk("Write Error...\n");
        goto out;
    }

    retval = count;

    printk("sema_test_write wait ...\n");
#ifdef USE_SEMA
    down(&sema_test_data.sem);
#endif
    printk("+++ sema_test_write +++\n");
    sema_test_data.val = val;
    printk("--- sema_test_write ---\n");
#ifdef USE_SEMA
    up(&sema_test_data.sem);
#endif

out:
    return retval;
}

ssize_t sema_test_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int retval;
	char val;


#ifdef USE_SEMA
	down(&sema_test_data.sem);
#endif
    printk("+++ sema_test_read +++\n");
    msleep(1000 * 10);
    val = sema_test_data.val;
    printk("--- sema_test_read ---\n");
#ifdef USE_SEMA
    up(&sema_test_data.sem);
#endif

    if (copy_to_user(buf, &val, count)) {
        retval = -EFAULT;
        printk("Read Error...\n");
                goto out;
    }

    retval = count;
out:
    return (retval);
}

static int sema_test_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "Call sema_test_open\n");	
	return 0;
}

static int sema_test_close(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Call sema_test_close\n");
    return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
    .open = sema_test_open,
    .release = sema_test_close,
	.read = sema_test_read,
	.write = sema_test_write,
};

static int sema_test_init(void) 
{ 
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t devt;

    alloc_ret = alloc_chrdev_region(&devt, 0, num_of_dev, DRIVER_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&sema_test_data.sema_test_cdev, &fops);
    cdev_ret = cdev_add(&sema_test_data.sema_test_cdev,devt, num_of_dev);
   	if(cdev_ret)
        goto error;

    sema_test_data.sema_test_class = class_create(THIS_MODULE, DRIVER_NAME);
    sema_test_data.sema_test_device = device_create(sema_test_data.sema_test_class,
        NULL, devt, NULL, "%s%d", DRIVER_NAME, 0);

    sema_init(&sema_test_data.sem, 1);

    sema_test_major = MAJOR(devt);
    printk(KERN_ALERT "%s driver(major %d) installed.\n", DRIVER_NAME, sema_test_major);

#ifdef USE_SEMA
    pr_info("USE_SEMA Enable\r\n");
#endif

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&sema_test_data.sema_test_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(devt, num_of_dev);

    return -1;
} 

static void sema_test_exit(void) 
{
    dev_t devt = MKDEV(sema_test_major, 0);

    cdev_del(&sema_test_data.sema_test_cdev);
    device_del(sema_test_data.sema_test_device);
    class_destroy(sema_test_data.sema_test_class);
    unregister_chrdev_region(devt, num_of_dev);
    printk(KERN_ALERT"%s driver removed\n",DRIVER_NAME); 
} 

module_init(sema_test_init); 
module_exit(sema_test_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is alloc_chrdev_region  module");
MODULE_VERSION("V1");
