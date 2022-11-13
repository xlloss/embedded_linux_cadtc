 
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h> 
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/io.h>
#include <linux/ioport.h>
/*
 *   pr_emerg()
 *   pr_alert()
 *   pr_crit()
 *   pr_err()
 *   pr_warning()
 *   pr_warn pr_warning()
 *   pr_notice()
 *   pr_info()
 *   pr_cont()
 */

#define PROC_DIR_NAME "led_info"
#define PROC_NAME "led"
#define BIT_W_EN(x) (16 + x * 2)
#define GPIO_A_SEL(x) (x * 2)

#define RK3X_PMUGRF_BASE 0xFF320000
#define RK3X_PMUGRF_GPIO0A_IOMUX RK3X_PMUGRF_BASE + 0x0
#define RK3X_GPIO_1_BASE 0xFF730000
#define RK3X_GPIO_1_SWPORTA_DR RK3X_GPIO_1_BASE + 0x0
#define RK3X_GPIO_1_SWPORTA_DDR RK3X_GPIO_1_BASE + 0x04

#define DEVICE_NAME "debug-led"
#define DEVICE_NUM 1
#define LED_NUM ARRAY_SIZE(led_gpios)
#define LED_ENABLE 0
#define LED_DISABLE 1

enum {
    GPIOA_0 = 0,
    GPIOA_1,
    GPIOA_2,
    GPIOA_3,
    GPIOA_4,
};

struct chrdev_rw_data {
    int id;
    struct class *chrdev_rw_class;
    struct device *chrdev_rw_device;
    unsigned int chrdev_rw_major;
    struct cdev chrdev_rw_cdev;
    struct proc_dir_entry *proc_dir;
    void __iomem *iomux;
    void __iomem *io_dat;
    void __iomem *io_dir;
};

struct chrdev_rw_data *_chrdev_rw_data;

int enable_dbg = 0;

int proc_open(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    file->private_data = _chrdev_rw_data;

    return 0;
}

int proc_close(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    return 0;
}

ssize_t proc_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    struct chrdev_rw_data *chrdev_rw;

    chrdev_rw = (struct chrdev_rw_data *)filp->private_data;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);


    if (enable_dbg) {
        pr_info("iomux 0x%x\r\n", readl(chrdev_rw->iomux));
        pr_info("io_dat 0x%x\r\n", readl(chrdev_rw->io_dat));
        pr_info("io_dir 0x%x\r\n", readl(chrdev_rw->io_dir));
    }

    return ret;
}

ssize_t proc_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    char val[50] = {0};

    ret = copy_from_user(&val, buf, count - 1);
    if(ret)
        return -EFAULT;

    if (!strcmp(val, "enable"))
        enable_dbg = 1;
    else
        enable_dbg = 0;

    return count;
}

struct file_operations proc_ops = {
    .open = proc_open,
    .release = proc_close,
    .read  = proc_read,
    .write  = proc_write,
};

ssize_t chrdev_rw_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret;
    unsigned char led_val = 0;
    struct chrdev_rw_data *chrdev_rw;

    chrdev_rw = (struct chrdev_rw_data *)filp->private_data;
    led_val = readb(chrdev_rw->io_dat) ;

    printk("Call chrdev_rw_read...\n");

    ret = copy_to_user(buf, &led_val, count);
    if(ret)
        return -EFAULT;

    return count;
}

ssize_t chrdev_rw_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    unsigned char chr_val = 0, led_sel = 0, led_val = 0, tmp;
    struct chrdev_rw_data *chrdev_rw;
    chrdev_rw = (struct chrdev_rw_data *)filp->private_data;

    printk("Call chrdev_rw_write...\n");

    ret = copy_from_user(&chr_val, buf, count);
    if(ret)
        return -EFAULT;

    printk("Call chrdev_rw_write... chr_val = 0x%x\n", chr_val);

    /**********************
    * 0000 CCVV
    *********************/

    led_sel = (chr_val & 0x0C) >> 2;
    led_val = chr_val & 0x01;

    printk("Call chrdev_rw_write... led_sel = 0x%x\n", led_sel);
    printk("Call chrdev_rw_write... led_val = 0x%x\n", led_val);
    tmp = readb(chrdev_rw->io_dat);

    switch (led_sel) {
    case 0:
        break;

    case 1:
        break;

    case 2:
        led_sel++;
        break;

    case 3:
        led_sel++;
        break;

    default:
        break;
    }

    tmp = tmp & ~(1 << led_sel);
    tmp = tmp | (led_val << led_sel);

    writeb(tmp, chrdev_rw->io_dat);

    return count;
}

static int chrdev_rw_open(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    _chrdev_rw_data->id = 10;
    file->private_data = _chrdev_rw_data;
    return 0;
}

static int chrdev_rw_close(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    return 0;
}

struct file_operations fops = {
    .owner	 = THIS_MODULE,
    .open	 = chrdev_rw_open,
    .release = chrdev_rw_close,
    .read 	 = chrdev_rw_read,
    .write 	 = chrdev_rw_write,
};

int led_init(struct chrdev_rw_data *chrdev)
{
    unsigned int gpioa_iomux_r, gpioa_dir_r, gpioa_dat_r;

    chrdev->iomux = ioremap(RK3X_PMUGRF_GPIO0A_IOMUX, 4);
    chrdev->io_dir = ioremap(RK3X_GPIO_1_SWPORTA_DDR, 4);
    chrdev->io_dat = ioremap(RK3X_GPIO_1_SWPORTA_DR, 4);

    gpioa_iomux_r = readl(chrdev->iomux);
    gpioa_iomux_r = gpioa_iomux_r | (0x03 << BIT_W_EN(0));
    gpioa_iomux_r = gpioa_iomux_r | (0x03 << BIT_W_EN(1));
    gpioa_iomux_r = gpioa_iomux_r | (0x03 << BIT_W_EN(3));
    gpioa_iomux_r = gpioa_iomux_r | (0x03 << BIT_W_EN(4));
    writel(gpioa_iomux_r, chrdev->iomux);

    gpioa_iomux_r = gpioa_iomux_r & ~(0x03 << GPIO_A_SEL(GPIOA_0));
    gpioa_iomux_r = gpioa_iomux_r & ~(0x03 << GPIO_A_SEL(GPIOA_1));
    gpioa_iomux_r = gpioa_iomux_r & ~(0x03 << GPIO_A_SEL(GPIOA_3));
    gpioa_iomux_r = gpioa_iomux_r & ~(0x03 << GPIO_A_SEL(GPIOA_4));
    writel(gpioa_iomux_r, chrdev->iomux);

    gpioa_dir_r = readl(chrdev->io_dir);
    gpioa_dir_r = gpioa_dir_r | ((1 << GPIOA_1) | (1 << GPIOA_0));
    gpioa_dir_r = gpioa_dir_r | ((1 << GPIOA_4) | (1 << GPIOA_3));
    writel(gpioa_dir_r, chrdev->io_dir);

    gpioa_dat_r = readl(chrdev->io_dat);
    gpioa_dat_r = gpioa_dat_r | ((1 << GPIOA_1) | (1 << GPIOA_0));
    gpioa_dat_r = gpioa_dat_r | ((1 << GPIOA_4) | (1 << GPIOA_3));
    writel(gpioa_dat_r, chrdev->io_dat);

    return 0;
}

void led_deinit(struct chrdev_rw_data *chrdev)
{
    unsigned int gpioa_dat_r;

    gpioa_dat_r = readl(chrdev->io_dat);
    gpioa_dat_r = gpioa_dat_r | ((1 << GPIOA_1) | (1 << GPIOA_0));
    gpioa_dat_r = gpioa_dat_r | ((1 << GPIOA_4) | (1 << GPIOA_3));
    writel(gpioa_dat_r, chrdev->io_dat);

    iounmap(chrdev->iomux);
    iounmap(chrdev->io_dir);
    iounmap(chrdev->io_dat);

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

}

static int __init led_dev_init(void) 
{
    int ret;
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t devt;
    struct class *dev_class;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    _chrdev_rw_data = kmalloc(sizeof(struct chrdev_rw_data), GFP_KERNEL);

    alloc_ret = alloc_chrdev_region(&devt, 0, DEVICE_NUM, DEVICE_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&(_chrdev_rw_data->chrdev_rw_cdev), &fops);
    cdev_ret = cdev_add(&(_chrdev_rw_data->chrdev_rw_cdev), devt, DEVICE_NUM);
    if (cdev_ret)
        goto error;

    _chrdev_rw_data->chrdev_rw_class = class_create(THIS_MODULE, DEVICE_NAME);

    dev_class = _chrdev_rw_data->chrdev_rw_class;
    _chrdev_rw_data->chrdev_rw_device = device_create(dev_class, NULL,
        devt, NULL, "%s-%d", DEVICE_NAME, 0);

    /* proc file */
    _chrdev_rw_data->proc_dir = proc_mkdir(PROC_DIR_NAME, NULL);
    proc_create(PROC_NAME, 666, _chrdev_rw_data->proc_dir, &proc_ops);

    ret = led_init(_chrdev_rw_data);
    if (ret < 0)
        pr_err("led_init() fail\r\n");

    _chrdev_rw_data->chrdev_rw_major = MAJOR(devt);
    printk(DEVICE_NAME"initialized\n");

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&_chrdev_rw_data->chrdev_rw_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(devt, DEVICE_NUM);

    kfree(_chrdev_rw_data);

	return ret;
}

static void __exit led_dev_exit(void) 
{
    dev_t dev;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    led_deinit(_chrdev_rw_data);

    dev = MKDEV(_chrdev_rw_data->chrdev_rw_major, 0);
    cdev_del(&(_chrdev_rw_data->chrdev_rw_cdev));
    device_del(_chrdev_rw_data->chrdev_rw_device);
    class_destroy(_chrdev_rw_data->chrdev_rw_class);
    unregister_chrdev_region(dev, DEVICE_NUM);

    remove_proc_entry(PROC_NAME, _chrdev_rw_data->proc_dir);
    remove_proc_entry(PROC_DIR_NAME, NULL);

    kfree(_chrdev_rw_data);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");

