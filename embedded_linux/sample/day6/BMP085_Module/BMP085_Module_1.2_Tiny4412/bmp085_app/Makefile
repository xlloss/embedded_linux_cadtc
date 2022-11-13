###########################################################################################
#       Tester Name: bmp085_test
#       Abstract: Makefile for bmp085 pressure sensor linux tester code
#       Author: Howard Lin
#       Contact:cdpda.teacher@gmail.com
#########################################################################################


.PHONY:bmp085_test
CC= arm-linux-gcc
SRC=bmp085_test
CFLAGS = -lm

all:
	$(CC) $(SRC).c -o $(SRC) $(CFLAGS)

install:
	cp $(SRC)  /home/cadtc/root_mkfs/bin

clean:
	rm $(SRC)
