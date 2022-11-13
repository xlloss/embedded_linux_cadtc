# ----------------------------------------------------------------------------
# Makefile for building tapp
#
# Copyright 2010 FriendlyARM (http://www.arm9.net/)
#

ifndef DESTDIR
DESTDIR			   ?= /home/cadtc/tiny4412/experiment/root_mkfs
endif

CFLAGS				= -Wall -O2 -static
CC					= arm-linux-gcc
INSTALL				= install

TARGET				= i2c


all: $(TARGET)

i2c: eeprog.o 24cXX.o
	$(CC) $(CFLAGS) $^ -o $@


install: $(TARGET)
	$(INSTALL) $^ $(DESTDIR)/usr/local/bin

clean distclean:
	rm -rf *.o $(TARGET)


# ----------------------------------------------------------------------------

.PHONY: $(PHONY) install clean distclean

# End of file
# vim: syntax=make

