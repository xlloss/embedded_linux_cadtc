sample_prog: src/sample_prog.o src/ctest1.o src/ctest2.o
	gcc -o sample_prog src/sample_prog.o src/ctest1.o src/ctest2.o

ctest1.o: src/ctest1.c
	gcc -c src/ctest1.c

ctest2.o: src/ctest2.c
	gcc -c src/ctest2.c

clean:
	rm src/*.o sample_prog
