CFLAGS = -Wall -O2 -static
CC = $(CROSS_COMPILE)gcc
INSTALL = install
TARGET = $(eeprom_i2c_test)

eeprom_i2c_test: at24c256.o
	$(CC) $(CFLAGS) $^ -o $@

clean distclean:
	rm -rf *.o eeprom_i2c_test


.PHONY: $(PHONY)clean distclean
