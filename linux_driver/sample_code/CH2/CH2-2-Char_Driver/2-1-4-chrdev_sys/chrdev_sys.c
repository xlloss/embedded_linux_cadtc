/* 
 * Copy from Terence and modify by Slash
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
#include <linux/device.h>
#include <linux/cdev.h>

#define DRIVER_NAME "chrdev_sys"
#define DEVICE_ID "0123456"
static unsigned int chrdev_sys_major = 0;
static struct class *chrdev_sys_class = NULL;
struct device *chrdev_sys_device;
struct device_attribute device_id_attr;
static char id_string[50];
static struct cdev cdev;

static int chrdev_sys_open(struct inode *inode, struct file *file)
{
	printk("Call chrdev_sys_open...\n");
	return 0;
}

static int chrdev_sys_close(struct inode *inode, struct file *file)
{
	printk("Call chrdev_sys_close\n");
	return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open    = chrdev_sys_open,
    .release = chrdev_sys_close,
};

static ssize_t device_id_show(struct device *class_dev,
			 struct device_attribute *attr,
			 char *buf)
{
	return snprintf(buf, strlen(id_string), "%s\r\n", id_string);
}

static ssize_t device_id_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    if (strlen(buf) < 50)
        strcpy(id_string, buf);

    return count;
}


static int chrdev_sys_init(void) 
{
    int alloc_ret;
    dev_t dev;
    int cdev_ret;

    strcpy(id_string, DEVICE_ID);

    alloc_ret = alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME);
    if(alloc_ret) {
        pr_err("alloc_chrdev_region fail\r\n");
        goto error;
    }

//    chrdev_sys_major = MAJOR(dev);
//    cdev_init(&cdev, &fops);
//    cdev_ret = cdev_add(&cdev, dev, 1);

    sysfs_attr_init(&device_id_attr.attr);
    device_id_attr.attr.name = "id";
    device_id_attr.attr.mode = S_IRUGO | S_IWUSR;
    device_id_attr.show = device_id_show;
    device_id_attr.store = device_id_store;


    /* Create sys class */
    chrdev_sys_class = class_create(THIS_MODULE, DRIVER_NAME); 
    if (IS_ERR(chrdev_sys_class))
        return -1;

    /* Register sysfs */
    chrdev_sys_device = device_create(chrdev_sys_class, NULL, dev,
        NULL, "chrdev_sys_info");

    device_create_file(chrdev_sys_device, &device_id_attr);

    pr_info("%s driver(major %d)(minor %d) installed.\n",
        DRIVER_NAME, MAJOR(dev), MINOR(dev));

error:
    return 0;
} 

static void chrdev_sys_exit(void) 
{
    dev_t dev = MKDEV(chrdev_sys_major, 0);

    printk("%s driver removed\n", DRIVER_NAME);

    /* Unregister class */
    device_destroy(chrdev_sys_class, dev);
    class_destroy(chrdev_sys_class);
    cdev_del(&cdev);
    unregister_chrdev_region(dev, 1);
} 

module_init(chrdev_sys_init); 
module_exit(chrdev_sys_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is sysfs test module");
MODULE_VERSION("V1");
