##########################################################################################
#       Module Name: bmp085
#       Abstract: Makefile for bmp085 pressure sensor linux driver on Kernel series 2.6
#	Author: Howard Lin
#	Contact:cdpda.teacher@gmail.com	
#########################################################################################

.PHONY: module install clean

KERNELDIR ?= ~/tiny4412/Day4/linux_3.5.0_tiny4412
MODULE_NAME =bmp085

obj-m := $(MODULE_NAME).o

PWD := $(shell pwd)

all: module install

module:
	 $(MAKE) -C $(KERNELDIR) M=$(PWD) modules
#install:
#	sudo cp $(MODULE_NAME).ko ~/tiny4412/Day1/root_mkfs
clean:
	 rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers
