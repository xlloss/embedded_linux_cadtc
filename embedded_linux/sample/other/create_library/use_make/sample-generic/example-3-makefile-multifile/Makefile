sample_prog: sample_prog.o ctest1.o ctest2.o
	gcc -o sample_prog sample_prog.o ctest1.o ctest2.o

ctest1.o: ctest1.c
	gcc -c ctest1.c

ctest2.o: ctest2.c
	gcc -c ctest2.c

clean:
	rm *.o sample_prog
