/*********************************************************************
 * Author: Slash <slash.linux.c@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 ********************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/io.h>
#include <linux/uaccess.h>


#define DEVICE_NAME "simple-bad-keys"
#define DRIVER_NAME "simple-bad-keys"
#define GPIO_NUM 1

struct irq_gpio {
	int irq[GPIO_NUM];
	struct gpio_desc *gpiod[GPIO_NUM];
};

struct irq_gpio irq_gpio;

unsigned char key_btn;
unsigned char irq_base;

static int key_open(struct inode *inode, struct file *file)
{
	file->private_data = &key_btn;
	return 0;
}

ssize_t key_read(struct file *filp, char __user *buf,
	size_t count, loff_t *f_pos)
{
	int ret;
	unsigned char *key_val;

	key_val = filp->private_data;
    pr_debug("%s key_val %d\n", __func__, *key_val);

	ret = copy_to_user(buf, key_val, count);
	if(ret)
		return -EFAULT;

	return ret;
}

static struct file_operations simple_irq_dev_fops = {
	.owner = THIS_MODULE,
	.open = key_open,
	.read = key_read,
};

static struct miscdevice simple_irq_dev = {
	.minor			= MISC_DYNAMIC_MINOR,
	.name			= DEVICE_NAME,
	.fops			= &simple_irq_dev_fops,
};

static irqreturn_t gpio_irq_isr(int irq, void *devid)
{
	unsigned char *key_val;
	int gpio_val;

	key_val = (unsigned char *)devid;

	gpio_val = ~(gpiod_get_value(irq_gpio.gpiod[irq - irq_gpio.irq[0]]));
	gpio_val = gpio_val & 0x01;

	*key_val = (*key_val) & gpio_val << (irq - irq_gpio.irq[0]);
	*key_val = (*key_val) | gpio_val << (irq - irq_gpio.irq[0]);
 
    *key_val = ~(*key_val) & 0x01;
    pr_debug("irq %d key_val %d\n", irq, *key_val);
	return 0;
}

static int simple_irq_probe(struct platform_device *pdev)
{
	int ret, i;
	char dt_gpio_name[15];

	for (i = 0; i < GPIO_NUM; i++) {
		irq_gpio.irq[i] = 0;
		sprintf(dt_gpio_name, "key%d", i);
		irq_gpio.gpiod[i] = devm_gpiod_get_optional(&pdev->dev,
			dt_gpio_name, GPIOD_IN);
		if (IS_ERR(irq_gpio.gpiod[i])) {
			ret = PTR_ERR(irq_gpio.gpiod[i]);
			if (ret != -EPROBE_DEFER)
				dev_err(&pdev->dev, "Faiirq to get %s GPIO: %d\n",
					dt_gpio_name, ret);
			goto gpio_error;
		}

		irq_gpio.irq[i] = gpiod_to_irq(irq_gpio.gpiod[i]);
		pr_info("irq_gpio.irq[%d] %d\r\n", i, irq_gpio.irq[i]);
		ret = devm_request_irq(&pdev->dev, irq_gpio.irq[i],
			gpio_irq_isr, IRQ_TYPE_EDGE_BOTH, "gpio-irq", &key_btn);
		if (ret) {
			dev_err(&pdev->dev, "Cannot request irq %d (error %d)\n",
				irq_gpio.irq[i], ret);
			return ret;
		}
		disable_irq_nosync(irq_gpio.irq[i]);
	}

	ret = misc_register(&simple_irq_dev);
	if (ret) {
		dev_err(&pdev->dev, "misc_register fail\r\n");
		return ret;
	}

	for (i = 0; i < GPIO_NUM; i++) {
		if (irq_gpio.irq[i])
			enable_irq(irq_gpio.irq[i]);
	}

	return 0;

gpio_error:
	for (i = 0; i < GPIO_NUM; i++) {
		if (irq_gpio.gpiod[i])
			gpiod_put(irq_gpio.gpiod[i]);
	}

	return ret;
}

static int simple_irq_remove(struct platform_device *pdev)
{
	int i;
	pr_info("simple_irq_remove\r\n");
	for (i = 0; i < GPIO_NUM; i++) {
		if (irq_gpio.irq[i])
			devm_free_irq(&pdev->dev, irq_gpio.irq[i], &key_btn);
	}

	misc_deregister(&simple_irq_dev);

	return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id simple_irq_of_match[] = {
	{ .compatible = "gpiokey,irq-sample", .data = NULL},
	{},
};
MODULE_DEVICE_TABLE(of, simple_irq_of_match);
#else
#define simple_irq_of_match NULL
#endif

static struct platform_driver simple_irq_platform_driver = {
	.probe = simple_irq_probe,
	.remove = simple_irq_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = simple_irq_of_match,
	},
};

static int __init simple_irq_dev_init(void)
{
	int ret;

	ret = platform_driver_register(&simple_irq_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register fail\r\n", __func__);
		return ret;
	}

	pr_info("%s initialized\n", __func__);

	return ret;
}

static void __exit simple_irq_dev_exit(void)
{
	platform_driver_unregister(&simple_irq_platform_driver);
}

module_init(simple_irq_dev_init);
module_exit(simple_irq_dev_exit);

MODULE_AUTHOR("Slash <slash.linux.c@gmail.com>");
MODULE_LICENSE("GPL");
