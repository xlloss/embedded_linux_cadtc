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
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>


#define DRIVER_NAME "MODULE_LED"
#define RK3X_GPIO_1_SWPORTA_DDR_OFF 0x04


void __iomem *gpioa_iomux, *gpioa_data, *gpioa_dir;

static int probe_led(struct platform_device *pdev)
{
    struct resource *res;
    unsigned int gpioa_iomux_r, gpioa_data_r, gpioa_dir_r;

    pr_info("%s\r\n", __func__);
//
//    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
//    pr_info("IORESOURCE_MEM-0 0x%llx\r\n", res->start);
//    gpioa_iomux = devm_ioremap_resource(&pdev->dev, res);
//
//    res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
//    gpioa_dir = devm_ioremap_resource(&pdev->dev, res);
//    pr_info("IORESOURCE_MEM-1 0x%llx\r\n", res->start);
//
//    gpioa_data = ioremap(res->start, resource_size(res));
//    gpioa_dir = gpioa_data + RK3X_GPIO_1_SWPORTA_DDR_OFF;
//
//    gpioa_iomux_r = readl(gpioa_iomux);
//    gpioa_iomux_r = gpioa_iomux_r | (1 << 17 | 1 << 16);
//    writel(gpioa_iomux_r, gpioa_iomux);
//
//    gpioa_iomux_r = gpioa_iomux_r & ~(1 << 1 | 1 << 0);
//    writel(gpioa_iomux_r, gpioa_iomux);
//
//    gpioa_dir_r = readl(gpioa_dir);
//    gpioa_dir_r = gpioa_dir_r | (1 << 0);
//    writel(gpioa_dir_r, gpioa_dir);
//
//    gpioa_data_r = readl(gpioa_data);
//    gpioa_data_r = gpioa_data_r & ~(1 << 0);
//    writel(gpioa_data_r, gpioa_data);

    return 0;
}

static int remove_led(struct platform_device *pdev)
{
//    unsigned int gpioa_data_r;
//    dev_info(&pdev->dev, "Led driver removed\n");
//
//    gpioa_data_r = readl(gpioa_data);
//    gpioa_data_r = gpioa_data_r | (1 << 0);
//    writel(gpioa_data_r, gpioa_data);
//
//    iounmap(gpioa_iomux);
//    iounmap(gpioa_dir);
    return 0;
}

//#ifdef CONFIG_OF
///* Match table for device tree binding */
//static const struct of_device_id led_of_match[] = {
//	{ .compatible = "sample,led-sample-dts", .data = NULL},
//	{},
//};
//MODULE_DEVICE_TABLE(of, led_of_match);
//#else
//#define led_of_match NULL
//#endif

//static struct platform_driver led_platform_driver = {
//	.probe = probe_led,
//	.remove = remove_led,
//	.driver = {
//		.name = DRIVER_NAME,
//		.of_match_table = led_of_match,
//	},
//};

static int __init led_init(void)
{
	int ret;

//	ret = platform_driver_register(&led_platform_driver);
//	if (ret) {
//		pr_err("%s platform_driver_register fail\r\n", __func__);
//		return ret;
//	}

	return 0;
}

static void __exit led_cleanup(void)
{
	//platform_driver_unregister(&led_platform_driver);
}

module_init(led_init);
module_exit(led_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slash.linux.c@gmail.com");
