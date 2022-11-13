
#KERNELDIR =
#INSTALLDIR = 
PWD := $(shell pwd)


obj-m := hello_file1.o hello_file2.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp hello_file1.ko hello_file2.ko $(INSTALLDIR)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
