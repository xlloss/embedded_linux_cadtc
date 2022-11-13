KERNELDIR = /home/slash/for_cadtc/rockpi4-b/rockpi-kernel
#INSTALLDIR =

obj-m := hello_world.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

install:
	scp ./hello_workd.ko $(INSTALLDIR)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
