CC=$(CROSS_COMPILE)gcc
exerise: main.o
	$(CC) -o $@ $^

main.o: main.c
	$(CC) -g -c $^

clean:
	rm main.o
	rm exerise

