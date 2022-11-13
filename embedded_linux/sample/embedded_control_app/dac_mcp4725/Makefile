#CFLAGS	= -Wall -O2 -static
#CC	= $(CROSS_COMPILE)gcc
TARGET	= $(mcp4725)


mcp4725: dac_device.o mcp4725.o
	$(CC) $(CFLAGS) $^ -o $@

clean distclean:
	rm -rf *.o mcp4725

.PHONY: $(PHONY) install clean distclean
