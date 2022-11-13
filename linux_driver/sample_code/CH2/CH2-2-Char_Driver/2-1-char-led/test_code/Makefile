CC=$(CROSS_COMPILE)gcc
file = ledtest
ALL:
	$(CC) -o $(file) $(file).c

install:
	scp $(file) $(INSTALLDIR)

clean:
	rm -rf $(file)

