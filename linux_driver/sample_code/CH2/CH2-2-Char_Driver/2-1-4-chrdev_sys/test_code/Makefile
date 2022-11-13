chrdev_sys_test : chrdev_sys_test.o
		$(CROSS_COMPILE)gcc -o chrdev_sys_test chrdev_sys_test.o

chrdev_sys_test.o : chrdev_sys_test.c
		$(CROSS_COMPILE)gcc -c chrdev_sys_test.c

clean:
	rm *.o chrdev_sys_test
