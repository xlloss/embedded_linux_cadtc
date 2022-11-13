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

#define DRIVER_NAME "chrdev_alloc"

#ifdef USE_MY_STRUCT
struct my_dev {
    struct cdev *cdev;
    void *private;
};

struct my_dev mydev;

#else
static struct cdev cdev;
#endif

unsigned int chrdev_alloc_major = 0;
unsigned int num_of_dev = 2;
unsigned int base_minor = 0;

static int chrdev_alloc_open(struct inode *inode, struct file *file)
{
    printk("Call chrdev_alloc_open\n");
    return 0;
}

static int chrdev_alloc_close(struct inode *inode, struct file *file)
{
    printk("Call chrdev_alloc_close\n");
    return 0;
}

ssize_t chrdev_alloc_write (struct file *f,
	const char __user *data, size_t size, loff_t *offset)
{
	printk("%s %d\n", __func__, __LINE__);
	return size;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = chrdev_alloc_open,
    .release = chrdev_alloc_close,
    .write = chrdev_alloc_write,
};

static int chrdev_alloc_init(void)
{
    int alloc_ret;
    int cdev_ret;
    int chrdev_alloc_minor;
    dev_t dev;


    alloc_ret = alloc_chrdev_region(&dev, base_minor, num_of_dev, DRIVER_NAME);
    if(alloc_ret)
        goto error;

#ifdef USE_MY_STRUCT
    mydev.cdev = cdev_alloc();
    mydev.cdev->owner = THIS_MODULE;
    mydev.cdev->ops = fops;
    cdev_ret = cdev_add(&mydev.cdev, dev, num_of_dev);
    if (cdev_ret)
        goto error;
#else
    cdev_init(&cdev, &fops);
    cdev_ret = cdev_add(&cdev, dev, num_of_dev);
    if (cdev_ret)
        goto error;
#endif

    chrdev_alloc_major = MAJOR(dev);
    chrdev_alloc_minor = MINOR(dev);

    printk("%s driver(major %d)(minor %d) installed.\n",
        DRIVER_NAME, chrdev_alloc_major, chrdev_alloc_minor);

    return 0;

error:
    if (cdev_ret == 0) {
#ifdef USE_MY_STRUCT
        cdev_del(mydev.cdev);
#else
        cdev_del(&cdev);
#endif
    }

    if (alloc_ret == 0)
        unregister_chrdev_region(dev, num_of_dev);

    return -1;
}

static void chrdev_alloc_exit(void)
{
    dev_t dev = MKDEV(chrdev_alloc_major, base_minor);

    printk("%s driver removed\n",DRIVER_NAME);

#ifdef USE_MY_STRUCT
    cdev_del(mydev.cdev);
#else
    cdev_del(&cdev);
#endif

    unregister_chrdev_region(dev, num_of_dev);
}

module_init(chrdev_alloc_init);
module_exit(chrdev_alloc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");
