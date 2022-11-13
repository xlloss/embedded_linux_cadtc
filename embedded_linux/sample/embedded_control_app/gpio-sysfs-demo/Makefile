SRCS=gpiolib.c gpiodemo.c
CFLAGS=
CC=$(CROSS_COMPILE)gcc

all: gpioctl gpiodemo

gpioctl: gpiolib.c
	$(CC) $(CFLAGS) -DCTL -o gpioctl gpiolib.c

gpiodemo: gpiodemo.c
	$(CC) $(CFLAGS) -o gpiodemo gpiolib.c gpiodemo.c -lpthread

install: all
	install -m 4755 gpiodemo $(INSTALL)
	install -m 4755 gpioctl $(INSTALL)

clean:
	-rm -f gpiodemo gpioctl
