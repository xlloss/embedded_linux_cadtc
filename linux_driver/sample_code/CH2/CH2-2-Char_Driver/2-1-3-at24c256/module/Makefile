#KERNELDIR =
#INSTALLDIR =
PWD := $(shell pwd)


obj-m := at24cxx.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	scp *.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


