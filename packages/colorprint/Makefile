PACKAGE ?= colorprint
CC ?= gcc
AR ?= ar
RM ?= rm -rf
TOUCH ?= touch
INSTALL_BIN ?= install -m0755
INSTALL_DIR ?= install -d -m0755
INSTALL_DATA ?= install -m0644
INSTALL_CONF ?= install -m0600

CFLAGS ?= -Werror -Wall -g
INCLUDE ?= -I./
LDFLAGS += -L./

BUILD_DIR ?= .
PKG_INSTALL_ROOT_DIR ?= .
PKG_INSTALL_SUB_DIR ?= 


SOURCES := colorprint.c
OBJS := $(SOURCES:.c=.o)
LIBNAME := libclprint.so

all:$(BUILD_DIR)/.prepare $(LIBNAME) demo

$(BUILD_DIR)/.prepare:
	$(TOUCH) $(BUILD_DIR)/.prepare

$(LIBNAME):$(OBJS)
	$(CC) $(CFLAGS) -shared -o $(LIBNAME) $(OBJS)

demo:$(LIBNAME) demo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lclprint -o demo demo.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC $< -o $@

$(PACKAGE)/install:$(LIBNAME)
	$(INSTALL_DIR) $(PKG_INSTALL_ROOT_DIR)/$(PKG_INSTALL_SUB_DIR)/usr/sbin
	$(INSTALL_BIN) $(LIBNAME) $(PKG_INSTALL_ROOT_DIR)/$(PKG_INSTALL_SUB_DIR)/usr/sbin


clean:
	$(RM) $(OBJS) demo.o $(LIBNAME) demo $(BUILD_DIR)/.prepare $(PKG_INSTALL_ROOT_DIR)/$(PKG_INSTALL_SUB_DIR)/usr/sbin/$(LIBNAME)

.PHONY : all clean prepare install