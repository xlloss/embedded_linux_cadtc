control_led:control_led.o
	${CROSS_COMPILE}gcc -o control_led control_led.o -lpthread

control_led.o:control_led.c
	${CROSS_COMPILE}gcc -c control_led.c

clean:
	rm *.o control_led

