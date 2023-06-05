CFLAGS		= -Wall -O2 -static
CC			= $(CROSS_COMPILE)gcc
INSTALL		= install
TARGET		= $(eeprom_i2c_test)


eeprom_i2c_test: eeprog.o 24cXX.o
	$(CC) $(CFLAGS) $^ -o $@


install: $(TARGET)
	scp i2c $(INSTALLDIR)

clean distclean:
	rm -rf *.o eeprom_i2c_test

.PHONY: $(PHONY) install clean distclean
