PACKAGE ?= libdemo
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

SOURCES := libdemo.c
OBJS := $(SOURCES:.c=.o)
TARGET := $(PACKAGE)
TARGET_LIB := libtmp.so

CFLAGS ?= -Werror -Wall -g
INCLUDE ?= -I./ -I$(PKG_INSTALL_ROOT_DIR)/usr/include
LDFLAGS += -L./ -L$(PKG_INSTALL_ROOT_DIR)/usr/lib
HIDDEN_FLAG := -DLIB_HIDDEN_TEST

all:$(CURRENT_DIR)/.prepare $(TARGET)

$(CURRENT_DIR)/.prepare:
	$(TOUCH) $(CURRENT_DIR)/.prepare

$(TARGET):$(OBJS) $(TARGET_LIB)
	$(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -ldl -ltmp -Wl,-rpath,'$$ORIGIN':$(PKG_INSTALL_ROOT_DIR)/usr/lib/ -Wl,--export-dynamic -Wl,--version-script,libtmp.map -o $(TARGET)

$(TARGET_LIB):libtmp.o
	$(CC) libtmp.o -shared $(CFLAGS) $(LDFLAGS) -Wl,--version-script,libtmp.map -o $(TARGET_LIB)

run:
	@echo "\n\033[32;1m----------------      running      --------------\033[0m"
	@exec $(CURRENT_DIR)/$(TARGET)
	@echo "\n\033[32;1m----------------        end        --------------\033[0m"

%.o : %.c
# $(HIDDEN_FLAG)
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC -DPKG_INSTALL_ROOT_DIR="\"$(PKG_INSTALL_ROOT_DIR)\"" $< -o $@

clean:
	$(RM) $(CURRENT_DIR)/.prepare $(TARGET) $(OBJS) libtmp.o $(TARGET_LIB)

.PHONY : all clean prepare run
