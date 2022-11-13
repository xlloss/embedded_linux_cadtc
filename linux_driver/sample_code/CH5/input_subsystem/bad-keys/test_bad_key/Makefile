read_bad_key : read_bad_key.o
	$(CROSS_COMPILE)gcc -o read_bad_key read_bad_key.o

read_bad_key.o : read_bad_key.c
	$(CROSS_COMPILE)gcc -c read_bad_key.c

clean:
	rm *.o read_bad_key
