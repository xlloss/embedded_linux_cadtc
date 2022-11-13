CC=${CROSS_COMPILE}gcc
control_buzzer: control_buzzer.o
	${CC} -o control_buzzer control_buzzer.o -lpthread

control_buzzer.o: control_buzzer.c
	${CC} -O1 -c control_buzzer.c

clean:
	rm *.o control_buzzer

