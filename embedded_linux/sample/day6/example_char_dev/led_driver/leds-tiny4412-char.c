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
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPM4_BAS 0x110002e0
#define GPM4_CON 0x0
#define GPM4_DAT 0x4
#define GPM4_PUD 0x8

struct tiny4412_led {
	void __iomem *gpm4_con, *gpm4_dat, *gpm4_pud;
	u32 r_gpm4_con, r_gpm4_pud, r_gpm4_dat;
	struct cdev *led_cdev;
	struct class *dev_class;
	struct device *dev;
	dev_t devid;
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

static void led_gpio_write(struct tiny4412_led *led)
{
	writel(led->r_gpm4_dat, led->gpm4_dat);
}

void led_light(int led_on)
{
	led->r_gpm4_dat = readl(led->gpm4_dat);
	if (led_on) {
		/* light */
		pr_err("LED Light\n");
		led->r_gpm4_dat &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		writel(led->r_gpm4_dat, led->gpm4_dat);
	} else {
		/* dark */
		pr_err("LED Dark\n");
		led->r_gpm4_dat |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
		writel(led->r_gpm4_dat, led->gpm4_dat);	
	}
}

static long tiny4412_leds_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	pr_err("%s cmd %d\n", __func__, cmd);

	switch (cmd) {
		case 0x12345:
			led_light(1);
			break;
		case 0x54321:
			led_light(0);
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

ssize_t tiny4412_leds_read(struct file *file, char __user *buf, size_t count,
		loff_t *offset)
{
	int ret;
	char tmp;
	struct tiny4412_led *led = file->private_data;

	pr_err("%s\n", __func__);
	
	if (count > 1)
		return -EINVAL;

	tmp = led->r_gpm4_dat & 0x0f;
	ret = copy_to_user(buf, &tmp, count) ? -EFAULT : 0;

	pr_err("tmp 0x%x, count = 0x%x\n", tmp, count);

	return ret;
}

ssize_t tiny4412_leds_write(struct file *file, const char __user *buf,
		size_t count, loff_t *offset)
{
	int ret = 0;
	char tmp;

	pr_err("%s count = 0x%x\n", __func__, count);

	if (count > 1)
		return -EINVAL;

	if (count == 1) {
		ret = copy_to_user(&tmp, buf, count);
		tmp &= 0x0f;
		pr_err("tmp 0x%x, count = 0x%x\n", tmp, count);

		led->r_gpm4_dat = tmp;
		led_gpio_write(led);
	}

	return ret;
}

int tiny4412_leds_open(struct inode *inode, struct file *file)
{
	file->private_data = led;

	pr_err("%s\n", __func__);

	return 0;
}

static int tiny4412_leds_release(struct inode *inode, struct file *file)
{
	pr_err("%s\n", __func__);

	return 0;
}

static struct file_operations tiny4412_led_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = tiny4412_leds_ioctl,
	.open = tiny4412_leds_open,
	.write = tiny4412_leds_write,
	.read = tiny4412_leds_read,
	.release = tiny4412_leds_release,
};


static int __init tiny4412_led_char_init(void)
{
	struct resource res;
	struct cdev *led_cdev = cdev_alloc( );
	int ret;
	dev_t devid;

	pr_debug("%s\n", __func__);
	
	led = kmalloc(sizeof(struct tiny4412_led), GFP_KERNEL);
	memset(led, 0, sizeof(struct tiny4412_led));
	res.start = GPM4_BAS;
	res.end = GPM4_BAS + GPM4_PUD - 1;

	led->gpm4_con = ioremap(res.start, resource_size(&res));
	led_gpio_init(led);
	
	ret = alloc_chrdev_region(&devid, 0, 1, "tiny4412_led");
	if (ret) {
		pr_err("alloc_chrdev_region fail\n");
		return -EINVAL;
	}
	
	led->devid = devid;
	led->led_cdev = led_cdev;
	led->led_cdev->ops = &tiny4412_led_dev_fops;
	cdev_add(led->led_cdev, devid, 1);

	led->dev_class = class_create(THIS_MODULE, "tiny-led-dev");
	if (IS_ERR(led->dev_class)) {
		ret = PTR_ERR(led->dev_class);
		return ret;
	}

	led->dev = device_create(led->dev_class, NULL,
				devid, NULL, "tiny-4412-%d", 0);

	led_light(1);
	return 0;
}

static void __exit tiny4412_led_char_exit(void)
{
	pr_debug("%s\n", __func__);

	device_destroy(led->dev_class, led->devid);
	class_destroy(led->dev_class);
	cdev_del(led->led_cdev);

	led_light(0);
	iounmap(led->gpm4_con);
	kfree(led);
}

module_init(tiny4412_led_char_init);
module_exit(tiny4412_led_char_exit);

MODULE_AUTHOR("Slash.Huang <slash.linux.c@gmail.com>");
MODULE_DESCRIPTION("Tiny4412 LED char driver");
MODULE_LICENSE("GPL");
