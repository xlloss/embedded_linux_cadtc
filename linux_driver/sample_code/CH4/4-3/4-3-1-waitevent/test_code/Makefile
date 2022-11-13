CC= $(CROSS_COMPILE)gcc

all : 
	$(CC)  -o waitevent_w waitevent_w.c
	$(CC)  -o waitevent_r waitevent_r.c

install :
	 scp waitevent_w waitevent_r $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions waitevent_r waitevent_w
