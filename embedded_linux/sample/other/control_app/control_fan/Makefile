control_fan:control_fan.o
	${CROSS_COMPILE}gcc -o control_fan control_fan.o -lpthread

control_fan.o:control_fan.c
	${CROSS_COMPILE}gcc -c control_fan.c


clean:
	rm *.o control_fan

