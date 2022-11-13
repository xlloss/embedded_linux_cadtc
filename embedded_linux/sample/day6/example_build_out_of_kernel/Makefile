ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m	:=leds-tiny4412_out_of_kernel.o
else
KERNELDIR ?=/home/cadtc/tiny4412/experiment/tiny4412-kernel/linux_3.5.0_tiny4412
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
