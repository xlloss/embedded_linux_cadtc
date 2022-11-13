

PWD := $(shell pwd)

obj-m := tasklet_irq.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	scp tasklet_irq.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


