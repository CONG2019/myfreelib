PACKAGE ?= sysv_shm_demo
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

SOURCES := sysv_shm_demo.c
OBJS := $(SOURCES:.c=.o)
TARGET := $(PACKAGE)
SHM_WRITER := sysv_shm_writer
SHM_READER := sysv_shm_reader

CFLAGS ?= -Werror -Wall -g
INCLUDE ?= -I./ -I$(PKG_INSTALL_ROOT_DIR)/usr/include
LDFLAGS += -L./ -L$(PKG_INSTALL_ROOT_DIR)/usr/lib

all:$(CURRENT_DIR)/.prepare $(TARGET) $(SHM_WRITER) $(SHM_READER)

$(CURRENT_DIR)/.prepare:
	$(TOUCH) $(CURRENT_DIR)/.prepare

$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -o $(TARGET)

$(SHM_WRITER):sysv_shm_writer.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $(SHM_WRITER) sysv_shm_writer.c

$(SHM_READER):sysv_shm_reader.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $(SHM_READER) sysv_shm_reader.c

run:
	@echo "\n\033[32;1m----------------      running      --------------\033[0m"
	@exec $(CURRENT_DIR)/$(TARGET) ${argv}
	@echo "\n\033[32;1m----------------        end        --------------\033[0m"

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	$(RM) $(CURRENT_DIR)/.prepare $(TARGET) $(OBJS) $(SHM_WRITER) $(SHM_READER)

.PHONY : all clean prepare run
