CC=$(CROSS_COMPILE)gcc

CFLAGS=-Wl,-Map,out.map -lpthread -lm

all: hello_world

hello_world: hello_world.o hello_world_2.o
	$(CC) -o hello_world hello_world.o hello_world_2.o

hello_world.o: hello_world.c
	$(CC) $(CFLAGS) -c hello_world.c

hello_world_2.o: hello_world_2.c
	$(CC) $(CFLAGS) -c hello_world_2.c

clean:
	rm -r *.o
	rm hello_world
