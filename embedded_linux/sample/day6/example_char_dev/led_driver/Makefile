ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m	:=leds-tiny4412-char.o
else
KERNELDIR ?=/home/xlloss/work/tiny-4412/build/linux_3.5.0_tiny4412/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

depend .depend dep:
	$(CC) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
