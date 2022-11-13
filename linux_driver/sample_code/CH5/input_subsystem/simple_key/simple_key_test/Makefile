CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

TARGENAME       = simple_keyget
TARGET          = $(TARGENAME)
DEFINES         += -D_DEBUG -DDEBUG
CPPFLAGS        += -MMD -MP $(DEFINES) $(INCLUDES)
#CFLAGS          += $(CPPFLAGS) $(ARCH)
#CXXFLAGS        += $(CFLAGS)
#LDFLAGS         +=
#LIBRARYDIR      = ../lib/
#LIBS            +=
#INCLUDES        += -I../include
#RESFLAGS        += $(DEFINES) $(INCLUDES)
LDDEPS          +=
SRC             = main.c
OBJECTS         = main.o

all: TARGET

TARGET:
	$(CC) -c $(SRC) $(INCLUDES)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
	@rm -f ./*.o
	@rm -f ./*.d
