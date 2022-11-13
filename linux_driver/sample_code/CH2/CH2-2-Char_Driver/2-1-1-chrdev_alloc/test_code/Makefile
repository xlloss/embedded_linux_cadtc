chrdev_region_alloc : chrdev_region_alloc.o
	$(CROSS_COMPILE)gcc -o chrdev_region_alloc chrdev_region_alloc.o

chrdev_region_alloc.o : chrdev_region_alloc.c
	$(CROSS_COMPILE)gcc -c chrdev_region_alloc.c


clean:
	rm *.o chrdev_region_alloc
