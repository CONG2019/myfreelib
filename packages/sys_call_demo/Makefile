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

define buildsub
$(1)/compile:
	@echo "\n\033[32;1m Building $(1)... ...\033[0m"
	$(MAKE) -C $(CURRENT_DIR)/$(1)
$(1)/clean:
	@echo "\033[31;1m Clean $(1)... ...\033[0m"
	$(MAKE) -C $(CURRENT_DIR)/$(1) clean
endef

PACKAGES_LIST=$(shell cat $(CURRENT_DIR)/packages.list)
TARGETS=$(foreach target, $(PACKAGES_LIST), $(target)/compile)
CLEAN_TARGETS=$(foreach target, $(PACKAGES_LIST), $(target)/clean)

all:$(CURRENT_DIR)/.prepare $(TARGETS)

$(foreach target, $(PACKAGES_LIST), $(eval $(call buildsub,$(target))))

$(CURRENT_DIR)/.prepare:
	$(TOUCH) $(CURRENT_DIR)/.prepare

clean:$(CLEAN_TARGETS)
	$(RM) $(CURRENT_DIR)/.prepare

.PHONY : all clean