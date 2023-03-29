obj-m += uevent.o

KERNEL_DIR ?= /lib/modules/`uname -r`/build
PWD := $(shell pwd)

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o  *.cmd *.ko *.mod .tmp_versions *.order *.symvers *.mod.c .[a-zA-Z0-9-_]*

deploy:
