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
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#define LED_NUM 4
#define LED_ON 1
#define LED_OFF 0

#define DEVICE_NAME "char-led"
#define DRIVER_NAME "char-leds"
#define DEVICE_NUM 1
#define LED_ENABLE 0
#define LED_DISABLE 1

struct chrdev_rw_data {
    int id;
    struct class *chrdev_rw_class;
    struct device *chrdev_rw_device;
    unsigned int chrdev_rw_major;
    struct cdev chrdev_rw_cdev;
    struct platform_device *pdev;
    int led[LED_NUM];
    struct gpio_desc *gpiod[LED_NUM];
};

struct chrdev_rw_data *_chrdev_rw_data;

enum {
    GPIOA_0 = 0,
    GPIOA_1,
    GPIOA_2,
    GPIOA_3,
    GPIOA_4,
};

ssize_t chrdev_rw_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret;
    unsigned char led_val = 0;
    struct chrdev_rw_data *chrdev_rw;

    chrdev_rw = (struct chrdev_rw_data *)filp->private_data;
//    led_val = readb(chrdev_rw->io_dat) ;

    printk("Call chrdev_rw_read...\n");

    ret = copy_to_user(buf, &led_val, count);
    if(ret)
        return -EFAULT;

    return ret;
}

ssize_t chrdev_rw_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    unsigned char chr_val, led_sel, led_val;
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

    gpiod_set_value(chrdev_rw->gpiod[led_sel], !led_val);

    return ret;
}


static int chrdev_rw_open(struct inode *inode, struct file *file)
{
    /* struct chrdev_rw_data *data;  */

    printk("Call chrdev_rw_open...\n");

    _chrdev_rw_data->id = 10;

    file->private_data = _chrdev_rw_data;

    return 0;
}

static int chrdev_rw_close(struct inode *inode, struct file *file)
{
    printk("Call chrdev_rw_close...\n");

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
    int ret, i;
    char dt_gpio_name[15];
    struct platform_device *pdev;

    pdev = chrdev->pdev;

    pr_info("Kernel Driver %s %d\n", __func__, __LINE__);

    for (i = 0; i < LED_NUM; i++) {
        sprintf(dt_gpio_name, "led%d", i);
        chrdev->gpiod[i] = devm_gpiod_get_optional(&pdev->dev,
            dt_gpio_name, GPIOD_OUT_LOW);

        if (IS_ERR(chrdev->gpiod[i])) {
            ret = PTR_ERR(chrdev->gpiod[i]);
            if (ret != -EPROBE_DEFER)
                dev_err(&pdev->dev, "Failed to get %s GPIO: %d\n",
                    dt_gpio_name, ret);
            goto gpio_error;
        }
    }

    return 0;

gpio_error:
	for (i = 0; i < LED_NUM; i++) {
		if (chrdev->gpiod[i])
			gpiod_put(chrdev->gpiod[i]);
	}
    return -1;
}

int led_deinit(struct chrdev_rw_data *chrdev)
{
    int i;

    for (i = 0; i < LED_NUM; i++)
        gpiod_set_value(chrdev->gpiod[i], 0);

    return 0;
}

static int led_dev_init(struct platform_device *pdev)
{
    int ret;
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t devt;
    struct class *dev_class;

    _chrdev_rw_data = kmalloc(sizeof(struct chrdev_rw_data), GFP_KERNEL);

    alloc_ret = alloc_chrdev_region(&devt, 0, DEVICE_NUM, DEVICE_NAME);
    if(alloc_ret)
        goto error;

    _chrdev_rw_data->pdev = pdev;

    cdev_init(&(_chrdev_rw_data->chrdev_rw_cdev), &fops);
    cdev_ret = cdev_add(&(_chrdev_rw_data->chrdev_rw_cdev), devt, DEVICE_NUM);
    if (cdev_ret)
        goto error;

    _chrdev_rw_data->chrdev_rw_class = class_create(THIS_MODULE, DEVICE_NAME);

    dev_class = _chrdev_rw_data->chrdev_rw_class;
    _chrdev_rw_data->chrdev_rw_device = device_create(dev_class, NULL,
        devt, NULL, "%s-%d", DEVICE_NAME, 0);

    cdev_ret = led_init(_chrdev_rw_data);
    if (cdev_ret) {
        pr_err("led_init fail\r\n");
    }

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

static void led_dev_exit(void)
{
    dev_t dev;

    led_deinit(_chrdev_rw_data);
    dev = MKDEV(_chrdev_rw_data->chrdev_rw_major, 0);
    cdev_del(&(_chrdev_rw_data->chrdev_rw_cdev));
    device_del(_chrdev_rw_data->chrdev_rw_device);
    class_destroy(_chrdev_rw_data->chrdev_rw_class);
    unregister_chrdev_region(dev, DEVICE_NUM);
    kfree(_chrdev_rw_data);
}

static int char_led_probe(struct platform_device *pdev)
{
    int ret;

    ret = led_dev_init(pdev);

    return ret;
}

static int char_led_remove(struct platform_device *op)
{
    led_dev_exit();

    return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id char_led_of_match[] = {
	{ .compatible = "char,led-sample", .data = NULL},
	{},
};
MODULE_DEVICE_TABLE(of, char_led_of_match);
#else
#define char_led_of_match NULL
#endif

static struct platform_driver char_led_platform_driver = {
    .probe = char_led_probe,
    .remove = char_led_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = char_led_of_match,
    },
};

static int __init gpio_led_dev_init(void)
{
    int ret;

    ret = platform_driver_register(&char_led_platform_driver);
    if (ret) {
        pr_err("%s platform_driver_register fail\r\n", __func__);
        return ret;
    }

    pr_info("%s initialized\n", __func__);

    return ret;
}

static void __exit gpio_led_dev_exit(void)
{
    platform_driver_unregister(&char_led_platform_driver);
}


module_init(gpio_led_dev_init);
module_exit(gpio_led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");
