PACKAGE ?= sys_call_demo
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

CFLAGS ?= -Werror -Wall -g
INCLUDE ?= -I./
LDFLAGS += -L./ -L$(PKG_INSTALL_ROOT_DIR)/usr/lib