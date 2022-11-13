sample_prog: sample_prog.o libctest.a
	gcc -o sample_prog sample_prog.c -L$(shell pwd) -lctest


libctest.a: ctest1.o ctest2.o
	gcc -c ctest1.c
	gcc -c ctest2.c
	ar -cvq libctest.a ctest1.o ctest2.o

clean:
	rm *.o sample_prog *.a
