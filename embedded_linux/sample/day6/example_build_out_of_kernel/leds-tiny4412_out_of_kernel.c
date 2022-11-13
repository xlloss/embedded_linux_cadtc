/* 
 * Tiny4412 LED Driver
 * Slash.Huang <slash.linux.c@gmail.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#define DEBUG
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mm.h>

#define GPM4_BAS 0x110002e0
#define GPM4_CON 0x0
#define GPM4_DAT 0x4
#define GPM4_PUD 0x8

struct tiny4412_led {
	void __iomem *gpm4_con, *gpm4_dat, *gpm4_pud;
	u32 r_gpm4_con, r_gpm4_pud, r_gpm4_dat;
};

struct tiny4412_led *led;

static void led_gpio_init(struct tiny4412_led *led)
{
	led->gpm4_dat = led->gpm4_con + GPM4_DAT;
	led->gpm4_pud = led->gpm4_con + GPM4_PUD;

	led->r_gpm4_con = readl(led->gpm4_con);
	led->r_gpm4_pud = readl(led->gpm4_pud);
	led->r_gpm4_dat = readl(led->gpm4_dat);

	led->r_gpm4_con &= ~((0xf << 0) | (0xf << 4) | (0xf << 8) | (0xf << 12));
	led->r_gpm4_con |= (0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12);

	writel(led->r_gpm4_con, led->gpm4_con);
	led->r_gpm4_pud &= ~((0x3 << 0) | (0x3 << 2) | (0x3 << 4) | (0x3 << 6));
}

void led_light(int led_on)
{
	if (led_on) {
		/* light */
		led->r_gpm4_dat = readl(led->gpm4_dat);
		led->r_gpm4_dat &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		writel(led->r_gpm4_dat, led->gpm4_dat);
	} else {
		/* dark */
		led->r_gpm4_dat |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
		writel(led->r_gpm4_dat, led->gpm4_dat);	
	}
}


static int __init tiny4412_led_init(void)
{
	struct resource res;

	pr_debug("%s\n", __func__);
	
	led = kmalloc(sizeof(struct tiny4412_led), GFP_KERNEL);
	memset(led, 0, sizeof(struct tiny4412_led));
	res.start = GPM4_BAS;
	res.end = GPM4_BAS + GPM4_PUD - 1;

	led->gpm4_con = ioremap(res.start, resource_size(&res));
	led_gpio_init(led);
	
	led_light(1);
	return 0;
}

static void __exit tiny4412_led_exit(void)
{
	pr_debug("%s\n", __func__);

	led_light(0);
	iounmap(led->gpm4_con);
	kfree(led);
}

module_init(tiny4412_led_init);
module_exit(tiny4412_led_exit);

MODULE_AUTHOR("Slash.Huang <slash.linux.c@gmail.com>");
MODULE_DESCRIPTION("Tiny4412 LED driver");
MODULE_LICENSE("GPL");
