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
#include <linux/wait.h>


#define DEVICE_NAME "irqs"
#define DRIVER_NAME "rockpi4b-irqs"

#define GPIO_KEY 4
#define GPIO_LED 4

struct irq_gpio {
	int irq[GPIO_KEY];
	struct gpio_desc *key_gpiod[GPIO_KEY];
	struct work_struct workq;
	unsigned char irq_trig;
	int led[GPIO_LED];
	struct gpio_desc *led_gpiod[GPIO_LED];

};

struct irq_gpio irq_gpio;

static struct file_operations rockpi4b_irq_dev_fops = {
	.owner			= THIS_MODULE,
};

static struct miscdevice rockpi4b_irq_dev = {
	.minor			= MISC_DYNAMIC_MINOR,
	.name			= DEVICE_NAME,
	.fops			= &rockpi4b_irq_dev_fops,
};

void sam_workqueue(struct work_struct *work)
{
	int i;

	pr_err("%s IRQ_TRIG %d\r\n", __func__, irq_gpio.irq_trig);

	for (i = 0; i < GPIO_LED; i++) {
		if (irq_gpio.irq[i] == irq_gpio.irq_trig)
			break;
	}

	pr_info("%s i -> %d\r\n", __func__, i);

	gpiod_set_value(irq_gpio.led_gpiod[i], 0);
	msleep(500);  /* sleep */
	gpiod_set_value(irq_gpio.led_gpiod[i], 1);
}

static irqreturn_t gpio_irq_isr(int irq, void *devid)
{
    struct irq_gpio *pirq_gpio;

	pirq_gpio = (struct irq_gpio *)devid;
	pirq_gpio->irq_trig = irq;
	pr_err("IRQ %d\r\n", irq);
	schedule_work(&pirq_gpio->workq);

	return 0;
}

static int rockpi4b_irq_probe(struct platform_device *pdev)
{
	int ret, i;
	char dt_gpio_key_name[15];
    char dt_gpio_led_name[15];

	for (i = 0; i < GPIO_KEY; i++) {
		irq_gpio.irq[i] = 0;
		sprintf(dt_gpio_key_name, "irq%d", i);
		irq_gpio.key_gpiod[i] = devm_gpiod_get_optional(&pdev->dev,
			dt_gpio_key_name, GPIOD_IN);
		if (IS_ERR(irq_gpio.key_gpiod[i])) {
			ret = PTR_ERR(irq_gpio.key_gpiod[i]);
			if (ret != -EPROBE_DEFER)
				dev_err(&pdev->dev, "Faiirq to get %s GPIO: %d\n",
					dt_gpio_key_name, ret);
			goto gpio_error;
		}

		irq_gpio.irq[i] = gpiod_to_irq(irq_gpio.key_gpiod[i]);
		ret = devm_request_irq(&pdev->dev, irq_gpio.irq[i],
			gpio_irq_isr, IRQ_TYPE_EDGE_FALLING, "gpio-irq", &irq_gpio);
		if (ret) {
			dev_err(&pdev->dev, "Cannot request irq %d (error %d)\n",
				irq_gpio.irq[i], ret);
			return ret;
		}
		disable_irq_nosync(irq_gpio.irq[i]);

		sprintf(dt_gpio_led_name, "led%d", i);
		irq_gpio.led_gpiod[i] = devm_gpiod_get_optional(&pdev->dev,
			dt_gpio_led_name, GPIOD_OUT_HIGH);
		if (IS_ERR(irq_gpio.led_gpiod[i])) {
			ret = PTR_ERR(irq_gpio.led_gpiod[i]);
			if (ret != -EPROBE_DEFER)
				dev_err(&pdev->dev, "Failed to get %s GPIO: %d\n",
					dt_gpio_led_name, ret);
			goto gpio_error;
		}

	}

	INIT_WORK(&irq_gpio.workq, sam_workqueue);

	ret = misc_register(&rockpi4b_irq_dev);
	if (ret) {
		dev_err(&pdev->dev, "misc_register fail\r\n");
		return ret;
	}

	for (i = 0; i < GPIO_KEY; i++) {
		if (irq_gpio.irq[i])
			enable_irq(irq_gpio.irq[i]);
	}

	return 0;

gpio_error:
	for (i = 0; i < GPIO_KEY; i++) {
		if (irq_gpio.key_gpiod[i])
			gpiod_put(irq_gpio.key_gpiod[i]);
	}

	return ret;
}

static int rockpi4b_irq_remove(struct platform_device *pdev)
{
	int i;


	for (i = 0; i < GPIO_KEY; i++) {
		if (irq_gpio.irq[i])
			devm_free_irq(&pdev->dev, irq_gpio.irq[i], &irq_gpio);
	}

    flush_scheduled_work();
	misc_deregister(&rockpi4b_irq_dev);

	return 0;
}

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id rockpi4b_irq_of_match[] = {
	{ .compatible = "rockpi4b,workqueue-sample", .data = NULL},
	{},
};
MODULE_DEVICE_TABLE(of, rockpi4b_irq_of_match);
#else
#define rockpi4b_irq_of_match NULL
#endif

static struct platform_driver rockpi4b_irq_platform_driver = {
	.probe = rockpi4b_irq_probe,
	.remove = rockpi4b_irq_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = rockpi4b_irq_of_match,
	},
};

static int __init rockpi4b_irq_dev_init(void)
{
	int ret;

	ret = platform_driver_register(&rockpi4b_irq_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register fail\r\n", __func__);
		return ret;
	}

	pr_info("%s initialized\n", __func__);

	return ret;
}

static void __exit rockpi4b_irq_dev_exit(void)
{
	platform_driver_unregister(&rockpi4b_irq_platform_driver);
}

module_init(rockpi4b_irq_dev_init);
module_exit(rockpi4b_irq_dev_exit);

MODULE_AUTHOR("Slash <slash.linux.c@gmail.com>");
MODULE_DESCRIPTION("rockpi4b GPIO-IRQ Driver");
MODULE_LICENSE("GPL");
