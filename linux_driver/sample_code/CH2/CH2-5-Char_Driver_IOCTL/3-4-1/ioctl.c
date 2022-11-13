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
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "ioctl_test.h"


#define DRIVER_NAME "ioctl"

static unsigned int ioctl_test_major = 0;
static unsigned int num_of_dev = 1;
static struct cdev ioctl_test_cdev;

struct ioctl_test_data {
    unsigned int val;
    rwlock_t lock;
};

long ioctl_test_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct ioctl_test_data *dev = file->private_data;
    int retval = 0;
    struct ioctl_arg rw_buf;

	printk("kernel driver %s %d\n", __func__, __LINE__);

    memset(&rw_buf, 0, sizeof(struct ioctl_arg));

    if(_IOC_TYPE(cmd) != IOC_MAGIC )
        return -ENOTTY;

    if (_IOC_NR(cmd) > IOCTL_VAL_MAXNR)
        return -ENOTTY;

    switch(cmd) {
        case IOCTL_VALRESET:
            if (!capable(CAP_SYS_ADMIN)) {
                    retval = -EPERM;
                    goto done;
            }
            printk("== Case [IOCTL_VALRESET] ==\n");
            break;

        case IOCTL_VALSET:
            if (!capable(CAP_SYS_ADMIN)) {
                    retval = -EPERM;
                    goto done;
            }

            if (copy_from_user(&rw_buf, (int __user *)arg,
                    sizeof(struct ioctl_arg))) {
                retval = -EFAULT;
                goto done;
            }

            write_lock(&dev->lock);
            dev->val = rw_buf.val;
            write_unlock(&dev->lock);
            printk("== Case [IOCTL_VALSET] val = [%d] ==\n", dev->val);
            break;

        case IOCTL_VALGET:
            printk("== Case [IOCTL_VALGET] val = [%d] ==\n", dev->val);
            read_lock(&dev->lock);
            rw_buf.val = dev->val;
            read_unlock(&dev->lock);

            if(copy_to_user((int __user *)arg, &rw_buf,
                sizeof(struct ioctl_arg))) {
                            retval = -EFAULT;
                            goto done;
            }
            break;


        case IOCTL_VALGET_INT:
            printk("== Case [IOCTL_VALGET_INT] val = [%d] ==\n", dev->val);
            retval = put_user(dev->val, (int __user *)arg);
            break;

        case IOCTL_VALSET_INT:
            if (!capable(CAP_SYS_ADMIN))
                return -EPERM;

            write_lock(&dev->lock);
            dev->val = arg;
            write_unlock(&dev->lock);
            printk("== Case [IOCTL_VALSET_INT] val = [%d] ==\n", dev->val);
            break;

        default:
            printk("===== Error Command!! =====\n");
            retval = -ENOTTY;
            break;
    }

done:
        return (retval);
}

static int ioctl_test_open(struct inode *inode, struct file *file)
{
    struct ioctl_test_data *dev;

    printk(KERN_ALERT "Call ioctl_test_open\n");

    dev = kmalloc(sizeof(struct ioctl_test_data), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    /* initialize members */
    rwlock_init(&dev->lock);
    dev->val = 10;
    file->private_data = dev;

    return 0;
}

static int ioctl_test_close(struct inode *inode, struct file *file)
{
    struct ioctl_test_data *dev = file->private_data;
    printk(KERN_ALERT "Call ioctl_test_close\n");

    if(dev)
        kfree(dev);

    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = ioctl_test_open,
    .release = ioctl_test_close,
    .unlocked_ioctl = ioctl_test_ioctl,
};

static int ioctl_test_init ( void )
{
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t dev;

    alloc_ret = alloc_chrdev_region(&dev, 0, num_of_dev, DRIVER_NAME);
    if (alloc_ret)
        goto error;

    cdev_init(&ioctl_test_cdev, &fops);
    cdev_ret = cdev_add(&ioctl_test_cdev, dev, num_of_dev);

    if (cdev_ret)
        goto error;

    ioctl_test_major = MAJOR(dev);
    printk(KERN_ALERT"%s driver(major %d) installed.\n", DRIVER_NAME,ioctl_test_major);

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&ioctl_test_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(dev,num_of_dev);

    return -1;
}

static void ioctl_test_exit(void)
{
    dev_t dev = MKDEV(ioctl_test_major, 0);

    cdev_del(&ioctl_test_cdev);
    unregister_chrdev_region(dev,num_of_dev);
    printk(KERN_ALERT"%s driver removed\n",DRIVER_NAME); 
}

module_init(ioctl_test_init);
module_exit(ioctl_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");
