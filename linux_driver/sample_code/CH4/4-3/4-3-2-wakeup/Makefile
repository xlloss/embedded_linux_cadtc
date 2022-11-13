
PWD := $(shell pwd)

obj-m := sleepone.o wakeupone.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	scp  wakeupone.ko sleepone.ko $(INSTALLDIR)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


