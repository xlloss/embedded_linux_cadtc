CC=$(CROSS_COMPILE)gcc

CFLAGS=-Wl,-Map,out.map -lpthread -lm

SRC = hello_world.c hello_world_2.c
DBG =./debug

all: hello_world

hello_world::$(SRC)
	gcc -o hello_world $(SRC)

hello_world::$(DBG)
	gcc -g -o hello_world $(SRC)

clean:
	rm hello_world
