test_eep_char_driver : test_eep_char_driver.o
		$(CROSS_COMPILE)gcc -o test_eep_char_driver test_eep_char_driver.o

test_eep_char_driver.o : test_eep_char_driver.c
		$(CROSS_COMPILE)gcc -c test_eep_char_driver.c

clean:
	rm *.o test_eep_char_driver
