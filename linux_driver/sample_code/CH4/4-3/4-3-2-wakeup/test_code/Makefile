CC= $(CROSS_COMPILE)gcc

all : 
	$(CC)  -o sleepone_test sleepone_test.c

install :
	sudo scp sleepone_test $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions sleepone_test
