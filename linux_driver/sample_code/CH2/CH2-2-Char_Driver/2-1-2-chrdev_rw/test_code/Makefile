chrdev_region_rw.c : chrdev_region_rw.o
	$(CROSS_COMPILE)gcc -o chrdev_region_rw chrdev_region_rw.o

chrdev_region_rw.o : chrdev_region_rw.c
	$(CROSS_COMPILE)gcc -c chrdev_region_rw.c


clean:
	rm *.o chrdev_region_rw
