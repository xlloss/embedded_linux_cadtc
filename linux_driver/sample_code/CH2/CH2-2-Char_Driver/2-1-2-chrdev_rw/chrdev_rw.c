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
#include <linux/slab.h> //For kmalloc

#define DRIVER_NAME "chrdev_rw"
#define DEVICE_NUM 1

static unsigned int chrdev_rw_major = 0;
static struct cdev chrdev_rw_cdev;

struct chrdev_rw_data {
    int id;
};

ssize_t chrdev_rw_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int ret;
    int chr_val;
    struct chrdev_rw_data *val;

    printk("Call chrdev_rw_read...\n");
    val = filp->private_data;
    chr_val = val->id;

    ret = copy_to_user(buf, &chr_val, count);
    if(ret)
        return -EFAULT;

    return ret;
}

ssize_t chrdev_rw_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    int ret;
    unsigned char chr_val;
    struct chrdev_rw_data *val;

    printk("Call chrdev_rw_write...\n");
    val = filp->private_data;

    ret = copy_from_user(&chr_val, buf, count);
    if(ret)
        return -EFAULT;

    val->id = chr_val;

    printk("%s chr_val %d\n", __func__, chr_val);
    return ret;
}

static int chrdev_rw_open(struct inode *inode, struct file *file)
{
    struct chrdev_rw_data *data;

    printk("Call chrdev_rw_open...\n");

    data = kmalloc(sizeof(struct chrdev_rw_data), GFP_KERNEL);
    data->id = 10;
    printk("Initial ID = [%d]\n", data->id);

    file->private_data = data;

    return 0;
}

static int chrdev_rw_close(struct inode *inode, struct file *file)
{
    struct chrdev_rw_data *data;

    printk("Call chrdev_rw_close...\n");

    data = file->private_data;
    kfree(data);

    return 0;
}

//ssize_t slash_write (struct file *f, const char __user *d, size_t, loff_t *off)
//{
//	pr_info("%s\n", __func__);
//	
//}

struct file_operations fops = {
    .owner	 = THIS_MODULE,
    .open	 = chrdev_rw_open,
    .release = chrdev_rw_close,
    .read 	 = chrdev_rw_read,
    .write 	 = chrdev_rw_write,
    //.write 	 = slash_write,
};

static int chrdev_rw_init(void)
{
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t dev;

    alloc_ret = alloc_chrdev_region(&dev, 0, DEVICE_NUM, DRIVER_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&chrdev_rw_cdev, &fops);

    cdev_ret = cdev_add(&chrdev_rw_cdev,dev, DEVICE_NUM);
    if (cdev_ret)
        goto error;

    chrdev_rw_major = MAJOR(dev);
    printk("%s driver(major %d) installed\n", DRIVER_NAME, chrdev_rw_major);

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&chrdev_rw_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(dev, DEVICE_NUM);

    return -1;
}

static void chrdev_rw_exit(void)
{
    dev_t dev = MKDEV(chrdev_rw_major, 0);

    printk("%s driver removed\n",DRIVER_NAME);

    cdev_del(&chrdev_rw_cdev);

    unregister_chrdev_region(dev,DEVICE_NUM);
}

module_init(chrdev_rw_init);
module_exit(chrdev_rw_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is read/write file operation");
