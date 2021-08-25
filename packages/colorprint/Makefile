PACKAGE ?= colorprint
MAJOR_VERSION := 1
SUB_VERSION := 0.1
CC ?= gcc
AR ?= ar
RM ?= rm -rf
TOUCH ?= touch
LN ?= ln
STRIP ?= strip
INSTALL_BIN ?= install -m0755
INSTALL_DIR ?= install -d -m0755
INSTALL_DATA ?= install -m0644
INSTALL_CONF ?= install -m0600

CURRENT_DIR := $(shell pwd)
BUILD_DIR ?= $(CURRENT_DIR)
PKG_INSTALL_ROOT_DIR ?= $(CURRENT_DIR)

SOURCES := colorprint.c
OBJS := $(SOURCES:.c=.o)
LIBNAME := libclprint.so
TARGET := $(LIBNAME).$(MAJOR_VERSION).$(SUB_VERSION)
STATIC_LIB := libclprint.a

CFLAGS ?= -Werror -Wall -g
ARFLAGS ?= -rs
INCLUDE ?= -I./
LDFLAGS += -L./ -L$(PKG_INSTALL_ROOT_DIR)/usr/lib

all:$(CURRENT_DIR)/.prepare $(TARGET) $(PACKAGE)/install demo

$(CURRENT_DIR)/.prepare:
	$(TOUCH) $(CURRENT_DIR)/.prepare

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIBNAME).$(MAJOR_VERSION) -o $(TARGET) $(OBJS)

demo:$(TARGET) demo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'/usr/lib:$(PKG_INSTALL_ROOT_DIR)/usr/lib/ -lclprint -o demo demo.o

static:$(STATIC_LIB)

$(STATIC_LIB):$(OBJS)
	$(AR) $(ARFLAGS) -c $(STATIC_LIB) $(OBJS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC $< -o $@

$(PACKAGE)/install:$(TARGET)
	@echo "\033[33;1m Install $(PACKAGE)... ... \033[0m"
	$(INSTALL_DIR) $(PKG_INSTALL_ROOT_DIR)/usr/lib
	$(INSTALL_DIR) $(PKG_INSTALL_ROOT_DIR)/usr/include
	$(INSTALL_BIN) $(TARGET) $(PKG_INSTALL_ROOT_DIR)/usr/lib
	$(STRIP) $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(TARGET)
	$(INSTALL_DATA) colorprint.h $(PKG_INSTALL_ROOT_DIR)/usr/include
	$(LN) -sf $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(TARGET) $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(LIBNAME).$(MAJOR_VERSION)
	$(LN) -sf $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(LIBNAME).$(MAJOR_VERSION) $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(LIBNAME)

clean:
	$(RM) $(OBJS) demo.o $(TARGET) demo $(CURRENT_DIR)/.prepare $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(TARGET) $(STATIC_LIB)
	$(RM) $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(LIBNAME).$(MAJOR_VERSION) $(PKG_INSTALL_ROOT_DIR)/usr/lib/$(LIBNAME)
	if [ $(PKG_INSTALL_ROOT_DIR) = $(CURRENT_DIR) ]; then $(RM) -rf $(PKG_INSTALL_ROOT_DIR)/usr; fi


.PHONY : all clean prepare install static