CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
DIR=$(shell 'pwd')


test : main.o libctest.a
	$(CC) -o test main.c -L $(DIR) -lctest

libctest.a : test1.o test2.o
	$(AR) -cvq libctest.a test1.o test2.o

test1.o : test1.c
	$(CC) -c test1.c

test2.o : test2.c
	$(CC) -c test2.c

.PHONY : clean
clean:
	rm test main.o test1.o test2.o libctest.a
