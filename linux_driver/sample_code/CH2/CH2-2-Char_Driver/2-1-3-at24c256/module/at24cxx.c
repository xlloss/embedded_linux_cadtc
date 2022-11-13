// SPDX-License-Identifier: GPL-2.0+
/*
 * at24cxx test driver
 * Copy from at24.c - handle most I2C EEPROMs
 *
 * Copyright (C) 2005-2007 David Brownell
 * Copyright (C) 2008 Wolfram Sang, Pengutronix
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/mod_devicetable.h>
#include <linux/log2.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/property.h>
#include <linux/acpi.h>
#include <linux/i2c.h>
#include <linux/nvmem-provider.h>
#include <linux/regmap.h>
#include <linux/pm_runtime.h>
#include <linux/gpio/consumer.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>

#define DRIVER_NAME "at24cxx"
#define DEVICE_NUM 1
#define DEVICE_NAME "at24cxx"

struct eep_cdev_data {
    int id;
    struct i2c_client *eep_i2c_client;
    struct class *eepcdev_class;
    struct device *eepcdev_device;
	unsigned int eep_cdev_maj;
	dev_t eep_devt;
};

struct eep_cdev_data *eep_data;

static struct cdev eep_cdev;

int i2c_write_read(const struct i2c_client *client, const char *buf)
{
	int ret;
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg[2];

	msg[0].addr = client->addr;
	msg[0].flags = client->flags & ~I2C_M_RD;
	msg[0].len = 2;
	msg[0].buf = (char *)&buf[0];

	msg[1].addr = client->addr;
	msg[1].flags = client->flags | I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = (char *)&buf[2];

	ret = i2c_transfer(adap, msg, 2);
	return ret;
}

int write_eep(struct i2c_client *client, unsigned char *val, loff_t count, loff_t offset)
{
	int ret, i;
	unsigned char data[3] = {0};

	data[0] = (unsigned char)offset;
	for (i = 0; i < count; i++) {
		msleep(1);
		data[2] = val[i];
		ret = i2c_master_send(client, data, 3);
		if (ret < 0) {
			dev_err(&client->dev, "%s i %d fail\n", __func__, i);
			return -1;
		}
		data[1] = data[1] + 1;
        msleep(1);
	}

	return 0;
}

int read_eep(struct i2c_client *client, unsigned char *val, size_t count, loff_t offset)
{
	int i;
	unsigned char buf[3] = {0};
	unsigned char address = 0;

	buf[0] = 0x00;
	buf[1] = offset;
	for (i = 0; i < count; i++) {
		msleep(1);
		i2c_write_read(client, buf);
		val[i] = buf[2];
		buf[1] = buf[1] + 1;
	}

	return 0;
}

static loff_t eep_cdev_llseek(struct file *file, loff_t offset, int whence)
{
	loff_t ret = -EINVAL;

	switch (whence) {
	case SEEK_END:
	case SEEK_DATA:
	case SEEK_HOLE:
		/* unsupported */
		return -EINVAL;
	case SEEK_CUR:
		if (offset == 0)
			return file->f_pos;

		offset += file->f_pos;
		break;
	case SEEK_SET:
		break;
	}

	if (offset != file->f_pos) {
		file->f_pos = offset;
		file->f_version = 0;
		ret = offset;
	}

	return ret;
}

#define MAX_BUF_SZ 256

ssize_t eep_cdev_read(struct file *filp, char __user *buf,
	size_t count, loff_t *f_pos)
{
	size_t ver_buf_sz, bytes_read = 0;
	unsigned char *data;
	ssize_t ret;

	pr_err("%s %d\r\n", __func__, (unsigned char)*f_pos);

	ver_buf_sz = min_t(size_t, count, MAX_BUF_SZ);
	data = kmalloc(ver_buf_sz, GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	ret = read_eep(eep_data->eep_i2c_client, data, count, *f_pos);
	if (ret) {
		pr_err("read_eep fail\r\n");
		return -1;
	}

	while (bytes_read < count) {
		if (copy_to_user(buf + bytes_read, data, ver_buf_sz)) {
			ret = -EFAULT;
			goto out;
		}
		bytes_read += ver_buf_sz;
		ver_buf_sz = min(count - bytes_read, (size_t)MAX_BUF_SZ);
	}

	(*f_pos) += bytes_read;
	ret = bytes_read;
out:
	kfree(data);
	return ret;
}


ssize_t eep_cdev_write(struct file *filp, const char __user *buf,
	size_t count, loff_t *f_pos)
{
	unsigned char *data;
	size_t buf_sz = 0, bytes_written = 0;
	int ret;

	pr_err("%s f_pos %d, count %d\r\n", __func__, (unsigned char)*f_pos, (unsigned int)count);
	buf_sz = min_t(size_t, count, MAX_BUF_SZ);
	data = kmalloc(buf_sz, GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	do {
		if (copy_from_user(data, &buf[bytes_written], buf_sz)) {
			ret = -EFAULT;
			goto out;
		}

		bytes_written += buf_sz;
		buf_sz = min(count - bytes_written, (size_t)MAX_BUF_SZ);
	} while (bytes_written < count);

	write_eep(eep_data->eep_i2c_client, data, count, *f_pos);

	(*f_pos) += bytes_written;
	ret = bytes_written;

out:
	kfree(data);
	return ret;
}

static int eep_cdev_open(struct inode *inode, struct file *file)
{
	printk("%s\r\n", __func__);
    return 0;
}

static int eep_cdev_close(struct inode *inode, struct file *file)
{
    printk("%s\r\n", __func__);
	return 0;
}


struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = eep_cdev_open,
    .release = eep_cdev_close,
    .llseek = eep_cdev_llseek,
    .read = eep_cdev_read,
    .write = eep_cdev_write,
};

static int at24_probe(struct i2c_client *client,
    const struct i2c_device_id *id)
{
	int ret;
	dev_t devt;

	dev_info(&client->dev, "%s\r\n", __func__);

    ret = alloc_chrdev_region(&devt, 0, DEVICE_NUM, DRIVER_NAME);
    if(ret) {
		dev_err(&client->dev, "alloc_chrdev_region fail\r\n");
        goto error_unregister_chrdev;
	}

	cdev_init(&eep_cdev, &fops);
    ret = cdev_add(&eep_cdev, devt, DEVICE_NUM);
    if (ret) {
		dev_err(&client->dev, "cdev_add fail\r\n");
        goto error_cdev_del;
	}

	eep_data = kmalloc(sizeof(struct eep_cdev_data), GFP_KERNEL);
	if (!eep_data) {
		dev_err(&client->dev, "alloc eep_data fail\r\n");
		goto error_cdev_del;
	}

	eep_data->eepcdev_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (!eep_data->eepcdev_class) {
		dev_err(&client->dev, "class_create fail\r\n");
		goto error_cdev_del;
	}

	eep_data->eepcdev_device = device_create(eep_data->eepcdev_class,
		NULL, devt, NULL, "%s-%d", DEVICE_NAME, 0);

	if (!eep_data->eepcdev_device) {
		dev_err(&client->dev, "device_create fail\r\n");
		goto error_class_create;
	}

	eep_data->eep_i2c_client = client;
	eep_data->eep_cdev_maj = MAJOR(devt);
	eep_data->eep_devt = devt;

	return 0;

error_class_create:
	class_destroy(eep_data->eepcdev_class);

error_cdev_del:
	cdev_del(&eep_cdev);
	kfree(eep_data);

error_unregister_chrdev:
	unregister_chrdev_region(devt, DEVICE_NUM);

    return -1;

}

static int at24_remove(struct i2c_client *client)
{
	dev_t devt = MKDEV(eep_data->eep_cdev_maj, 0);

	dev_info(&client->dev, "%s\r\n", __func__);

    cdev_del(&eep_cdev);
    unregister_chrdev_region(devt, DEVICE_NUM);
    device_del(eep_data->eepcdev_device);
    class_destroy(eep_data->eepcdev_class);
    kfree(eep_data);
	return 0;
}

static const struct i2c_device_id at24_ids[] = {
	/* needs 8 addresses as A0-A2 are ignored */
	{ "at24cxx", 0},
	{ /* END OF LIST */ }
};
MODULE_DEVICE_TABLE(i2c, at24_ids);

static struct i2c_driver at24_driver = {
	.driver = {
		.name = "at24cxx",
	},
	.probe = at24_probe,
	.remove = at24_remove,
	.id_table = at24_ids,
};

static int __init at24_init(void)
{
	return i2c_add_driver(&at24_driver);
}
module_init(at24_init);

static void __exit at24_exit(void)
{
	i2c_del_driver(&at24_driver);
}
module_exit(at24_exit);

MODULE_DESCRIPTION("Driver for most I2C EEPROMs");
MODULE_AUTHOR("slash.linux.c@gmail.com");
MODULE_LICENSE("GPL");
