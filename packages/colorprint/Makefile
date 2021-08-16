PACKAGE ?= colorprint
CC ?= gcc
AR ?= ar
RM ?= rm
CFLAGS ?= -Werror -Wall -g
INCLUDE ?= -I./
LDFLAGS += -L./

SOURCES := colorprint.c
OBJS := $(SOURCES:.c=.o)
LIBNAME := libclprint.so

all:$(LIBNAME) demo

$(LIBNAME):$(OBJS)
	$(CC) $(CFLAGS) -shared -o $(LIBNAME) $(OBJS)

demo:$(LIBNAME) demo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lclprint -o demo demo.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC $< -o $@

clean:
	$(RM) $(OBJS) demo.o $(LIBNAME) demo

.PHONY : all clean