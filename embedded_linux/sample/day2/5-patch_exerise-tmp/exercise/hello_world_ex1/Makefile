CC=$(CROSS_COMPILE)gcc

all: hello_world

hello_world: hello_world.o
	$(CC) -o hello_world hello_world.o

hello_world.o: hello_world.c
	$(CC) $(CFLAGS) -c hello_world.c

clean:
	rm -r *.o
	rm hello_world
