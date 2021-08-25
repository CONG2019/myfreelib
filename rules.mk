CC := gcc
CXX := g++
AR := ar -rsc
RM := rm -rf
TOUCH := touch
LN := ln -f
CP := cp -rf
INSTALL_BIN := install -m0755
INSTALL_DIR := install -d -m0755
INSTALL_DATA := install -m0644
INSTALL_CONF := install -m0600

CURRENT_DIR := $(shell pwd)
BUILD_DIR := $(CURRENT_DIR)/build_dir
PKG_INSTALL_ROOT_DIR := $(BUILD_DIR)
