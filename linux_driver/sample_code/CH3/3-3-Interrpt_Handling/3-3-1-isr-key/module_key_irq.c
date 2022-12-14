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


#define DEVICE_NAME "irqs"
#define DRIVER_NAME "gpiokey-irqs"

#define GPIO_NUM 2

struct irq_gpio {
	int irq[GPIO_NUM];
	struct gpio_desc *gpiod[GPIO_NUM];
};

struct irq_gpio irq_gpio;

static struct file_operations gpiokey_irq_dev_fops = {
	.owner			= THIS_MODULE,
};

static struct miscdevice gpiokey_irq_dev = {
	.minor			= MISC_DYNAMIC_MINOR,
	.name			= DEVICE_NAME,
	.fops			= &gpiokey_irq_dev_fops,
};

static irqreturn_t gpio_irq_isr(int irq, void *devid)
{
	int *gpio_irq_num;

	gpio_irq_num = (int *)devid;
	pr_err("IRQ %d, gpio_irq_num %d\r\n", irq, *gpio_irq_num);

	return 0;
}

static int gpiokey_irq_probe(struct platform_device *pdev)
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
			gpio_irq_isr, IRQ_TYPE_EDGE_FALLING, "gpio-irq", &irq_gpio.irq[i]);
		if (ret) {
			dev_err(&pdev->dev, "Cannot request irq %d (error %d)\n",
				irq_gpio.irq[i], ret);
			return ret;
		}
		disable_irq_nosync(irq_gpio.irq[i]);
	}

	ret = misc_register(&gpiokey_irq_dev);
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

static int gpiokey_irq_remove(struct platform_device *pdev)
{
	int i;


	for (i = 0; i < GPIO_NUM; i++) {
		if (irq_gpio.irq[i])
			devm_free_irq(&pdev->dev, irq_gpio.irq[i], &irq_gpio.irq[i]);
	}

	misc_deregister(&gpiokey_irq_dev);

	return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id gpiokey_irq_of_match[] = {
	{ .compatible = "gpiokey,irq-sample", .data = NULL},
	{},
};
MODULE_DEVICE_TABLE(of, gpiokey_irq_of_match);
#else
#define gpiokey_irq_of_match NULL
#endif

static struct platform_driver gpiokey_irq_platform_driver = {
	.probe = gpiokey_irq_probe,
	.remove = gpiokey_irq_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = gpiokey_irq_of_match,
	},
};

static int __init gpiokey_irq_dev_init(void)
{
	int ret;

	ret = platform_driver_register(&gpiokey_irq_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register fail\r\n", __func__);
		return ret;
	}

	pr_info("%s initialized\n", __func__);

	return ret;
}

static void __exit gpiokey_irq_dev_exit(void)
{
	platform_driver_unregister(&gpiokey_irq_platform_driver);
}

module_init(gpiokey_irq_dev_init);
module_exit(gpiokey_irq_dev_exit);

MODULE_AUTHOR("Slash <slash.linux.c@gmail.com>");
MODULE_LICENSE("GPL");
