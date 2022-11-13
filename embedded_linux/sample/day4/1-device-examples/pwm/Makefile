# ----------------------------------------------------------------------------
# Makefile for building tapp
#
# Copyright 2010 FriendlyARM (http://www.arm9.net/)
# Modify by Slash
#

ifndef DESTDIR
DESTDIR			   ?= /home/cadtc/tiny4412/experiment/root_mkfs
endif

CFLAGS				= -Wall -O2 -static
CC					= arm-linux-gcc
INSTALL				= install

TARGET				= pwm_test


all: $(TARGET)

pwm_test: pwm_test.c
	$(CC) $(CFLAGS) $< -o $@


install: $(TARGET)
	$(INSTALL) $^ $(DESTDIR)/usr/local/bin

clean distclean:
	rm -rf *.o $(TARGET)


# ----------------------------------------------------------------------------

.PHONY: $(PHONY) install clean distclean

# End of file
# vim: syntax=make

