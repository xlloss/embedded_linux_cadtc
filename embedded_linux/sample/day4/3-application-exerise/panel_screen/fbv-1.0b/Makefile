#
# Makefile
#
# Makefile for fbv

include Make.conf

CC	= arm-linux-gcc
CFLAGS  = -O2 -Wall -D_GNU_SOURCE -I/home/cadtc/tiny4412/experiment/day4/3-application-exerise/panel_screen/fbv-1.0b/

SOURCES	= main.c jpeg.c gif.c png.c bmp.c fb_display.c transforms.c
OBJECTS	= ${SOURCES:.c=.o}

OUT	= fbv
#LIBS	= -lungif -L/usr/X11R6/lib -ljpeg -lpng
LIBS	= -L/home/cadtc/tiny4412/experiment/day4/3-application-exerise/panel_screen/fbv-1.0b/ -ljpeg -lpng -lungif

all: $(OUT)
	@echo Build DONE.

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) *~ $$$$~* *.bak core config.log $(OUT)

distclean: clean
	@echo -e "error:\n\t@echo Please run ./configure first..." >Make.conf
	rm -f $(OUT) config.h

install: $(OUT)
	cp $(OUT) $(DESTDIR)$(bindir)
	gzip -9c $(OUT).1 > $(DESTDIR)$(mandir)/man1/$(OUT).1.gz

uninstall: $(DESTDIR)$(bindir)/$(OUT)
	rm -f $(DESTDIR)$(bindir)/$(OUT)
	rm -f $(DESTDIR)$(mandir)/man1/$(OUT).1.gz
