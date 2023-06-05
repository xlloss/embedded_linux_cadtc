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


#define DEVICE_NAME "helloworld"
#define DRIVER_NAME "plat-drv-helloworld"

struct led_gpio {
	struct gpio_desc *gpiod;
};

struct led_gpio led_gpio;

static int helloworld_probe(struct platform_device *pdev)
{
	u32 out_value;
	char dt_gpio_name[15];

	dev_info(&pdev->dev, "%s %d\n", __func__, __LINE__);

	of_property_read_u32(pdev->dev.of_node,
						"test-val",
						&out_value);


	sprintf(dt_gpio_name, "led%d", 0);
	led_gpio.gpiod = devm_gpiod_get_optional(&pdev->dev,
			dt_gpio_name, GPIOD_OUT_HIGH);

	gpiod_set_value(led_gpio.gpiod, 0);

	dev_info(&pdev->dev, "%s %d out_value=%d\n", __func__, __LINE__, out_value);
	printk("%s %d out_value %d\n", __func__, __LINE__, out_value);
	pr_info("%s %d out_value %d\n", __func__, __LINE__, out_value);

    return 0;
}

static int helloworld_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "%s %d\r\n", __func__, __LINE__);
	gpiod_set_value(led_gpio.gpiod, 1);

	return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id helloworld_of_match[] = {
    { .compatible = "sample-test,platform-driver", .data = NULL},
    {},
};
MODULE_DEVICE_TABLE(of, helloworld_of_match);
#else
#define helloworld_of_match NULL
#endif

static struct platform_driver helloworld_platform_driver = {
    .probe = helloworld_probe,
    .remove = helloworld_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = helloworld_of_match,
    },
};

static int __init helloworld_dev_init(void)
{
	int ret;

	ret = platform_driver_register(&helloworld_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register fail\r\n", __func__);
		return ret;
	}

	pr_info("%s initialized\n", __func__);

	return ret;
}

static void __exit helloworld_dev_exit(void)
{
	platform_driver_unregister(&helloworld_platform_driver);
}

module_init(helloworld_dev_init);
module_exit(helloworld_dev_exit);

MODULE_AUTHOR("Slash <slash.linux.c@gmail.com>");
MODULE_LICENSE("GPL");
