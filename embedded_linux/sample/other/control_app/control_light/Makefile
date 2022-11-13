control_light:control_light.o
	${CROSS_COMPILE}gcc -o control_light control_light.o -lpthread

control_light.o:control_light.c
	${CROSS_COMPILE}gcc -c control_light.c


clean:
	rm *.o control_light

