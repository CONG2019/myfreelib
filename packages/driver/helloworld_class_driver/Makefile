obj-m += helloworld_class_driver.o
obj1=ioctl_test

KERNEL_DIR ?= /lib/modules/`uname -r`/build
PWD := $(shell pwd)

all:$(obj1)
	make -C $(KERNEL_DIR) M=$(PWD) modules

$(obj1): ioctl_test.c
	gcc ioctl_test.c -o $(obj1)

clean:
	rm -rf *.o  *.cmd *.ko *.mod .tmp_versions *.order *.symvers *.mod.c .[a-zA-Z0-9-_]* $(obj1)

deploy:
