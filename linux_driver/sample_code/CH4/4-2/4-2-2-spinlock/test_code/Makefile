CC=$(CROSS_COMPILE)gcc

all :
	$(CC) -o test_read_write test_read_write.c -lpthread

install :
	sudo cp test_read_write $(INSTALLDIR)

clean:
	rm test_read_write
