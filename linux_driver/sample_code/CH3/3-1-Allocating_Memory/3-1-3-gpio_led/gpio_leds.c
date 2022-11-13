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


#define DEVICE_NAME "leds"
#define DRIVER_NAME "gpio-leds"

#define LED_NUM 4
#define LED_ON 1
#define LED_OFF 0

struct led_gpio {
	int led[LED_NUM];
	struct gpio_desc *gpiod[LED_NUM];
};

struct led_gpio led_gpio;

static long leds_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	/* pr_err("%s cmd %d, arg 0x%x\r\n", __func__, cmd, arg); */

	switch (cmd) {
		case LED_ON:
		case LED_OFF:
			if (arg > LED_NUM) {
				return -EINVAL;
			}

			gpiod_set_value(led_gpio.gpiod[arg], !cmd);
			break;

		default:
			return -EINVAL;
	}

	return 0;
}

static struct file_operations led_dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= leds_ioctl,
};

static struct miscdevice led_dev = {
	.minor			= MISC_DYNAMIC_MINOR,
	.name			= DEVICE_NAME,
	.fops			= &led_dev_fops,
};

static int led_probe(struct platform_device *pdev)
{
	int ret, i;
	char dt_gpio_name[15];

	for (i = 0; i < LED_NUM; i++) {
		sprintf(dt_gpio_name, "led%d", i);
		led_gpio.gpiod[i] = devm_gpiod_get_optional(&pdev->dev,
			dt_gpio_name, GPIOD_OUT_HIGH);
		if (IS_ERR(led_gpio.gpiod[i])) {
			ret = PTR_ERR(led_gpio.gpiod[i]);
			if (ret != -EPROBE_DEFER)
				dev_err(&pdev->dev, "Failed to get %s GPIO: %d\n",
					dt_gpio_name, ret);
			goto gpio_error;
		}
	}

	ret = misc_register(&led_dev);
	if (ret) {
		dev_err(&pdev->dev, "misc_register fail\r\n");
		return ret;
	}

	return 0;

gpio_error:
	for (i = 0; i < LED_NUM; i++) {
		if (led_gpio.gpiod[i])
			gpiod_put(led_gpio.gpiod[i]);
	}

	return ret;
}

static int led_remove(struct platform_device *op)
{
	int i;

	for (i = 0; i < LED_NUM; i++)
		gpiod_set_value(led_gpio.gpiod[i], 0);

	misc_deregister(&led_dev);

	return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id led_of_match[] = {
	{ .compatible = "sample,gpio-led", .data = NULL},
	{},
};
MODULE_DEVICE_TABLE(of, led_of_match);
#else
#define led_of_match NULL
#endif

static struct platform_driver led_platform_driver = {
	.probe = led_probe,
	.remove = led_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = led_of_match,
	},
};

static int __init led_dev_init(void)
{
	int ret;

	ret = platform_driver_register(&led_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register fail\r\n", __func__);
		return ret;
	}

	pr_info("%s initialized\n", __func__);

	return ret;
}

static void __exit led_dev_exit(void)
{
	platform_driver_unregister(&led_platform_driver);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_AUTHOR("Slash <slash.linux.c@gmail.com>");
MODULE_LICENSE("GPL");
