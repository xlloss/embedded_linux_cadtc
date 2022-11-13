#KERNELDIR =
#INSTALLDIR =
PWD := $(shell pwd)
obj-m := simple_keys.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
