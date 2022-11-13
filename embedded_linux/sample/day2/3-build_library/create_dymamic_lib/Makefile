CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
DIR=$(shell 'pwd')

test : main.o libctest.so
	$(CC) -o test main.c -L $(DIR) -lctest

libctest.so : test1.o test2.o
	$(CC) -shared -Wl,-soname,libctest.so.1 -o libctest.so.1.0 test1.o test2.o
	ln -s libctest.so.1.0 libctest.so
	ln -s libctest.so.1.0 libctest.so.1
test1.o : test1.c
	$(CC) -c test1.c

test2.o : test2.c
	$(CC) -c test2.c

.PHONY : clean
clean:
	rm test main.o test1.o test2.o libctest.so.1 libctest.so.1.0 libctest.so
