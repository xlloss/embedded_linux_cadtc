#KERNELDIR =
#INSTALLDIR =
PWD := $(shell pwd)
DEBUG = y

ifeq ($(DEBUG),y)
   DEBFLAGS = -O -g -DHELLO_DEBUG # "-O" is needed to expand inlines
else
   DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS)

obj-m := hello_debug.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	scp *.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
