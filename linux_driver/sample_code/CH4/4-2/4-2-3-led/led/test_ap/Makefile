test_eep_char_driver : test_led_char_driver.o
		$(CROSS_COMPILE)gcc -o test_led_char_driver test_led_char_driver.o

test_led_char_driver.o : test_led_char_driver.c
		$(CROSS_COMPILE)gcc -c test_led_char_driver.c

clean:
	rm *.o test_led_char_driver
