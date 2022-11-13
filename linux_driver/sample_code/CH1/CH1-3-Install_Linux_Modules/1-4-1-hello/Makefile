#KERNELDIR =
#INSTALLDIR =

obj-m := hello_world_v3.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

install:
	scp ./*.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
