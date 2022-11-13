PWD := $(shell pwd)

obj-m := timer.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	 scp timer.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


