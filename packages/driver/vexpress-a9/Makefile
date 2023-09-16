MACHINA="vexpress-a9"
IMAGE_DIR=/home/cong/work/images/vexpress-a9
SRC_DIR=/work

DTB=${IMAGE_DIR}/vexpress-v2p-ca9.dtb
KERNEL=${IMAGE_DIR}/zImage
SD_ROOTFS=${IMAGE_DIR}/a9rootfs.ext3

MEM="512M"
APPEND="root=/dev/mmcblk0 rw console=ttyAMA0"

_src=$(filter-out %.mod.c,$(wildcard ${SRC_DIR}/*.c))
obj-m = $(patsubst ${SRC_DIR}/%.c,%.o,${_src})
KERNEL_DIR ?= /kernel/build		# for kernel src is not the build dir
PWD := $(shell pwd)

TAP="tap0"
BRIDGE="br0"
HOST_IP="192.168.99.1"
MACHINA_IP="192.168.99.2"
TFTP_CMD := atftp
ifeq ($(NET), 1)
	NET_OPT += -nic tap,id=net0,ifname=${TAP}
else
	NET_OPT = 
endif

net-init:.IGNORE
	sudo brctl addbr ${BRIDGE}
	sudo ifconfig ${BRIDGE} up
	sudo ifconfig ${BRIDGE} ${HOST_IP}

	sudo tunctl -t ${TAP} -u root
	sudo brctl addif ${BRIDGE} ${TAP}
	sudo ifconfig ${TAP} 0.0.0.0 promisc up

net-exit:.IGNORE
	sudo ip link delete ${TAP} > /dev/null
	sudo ifconfig ${BRIDGE} down
	sudo brctl delbr ${BRIDGE}

init:.IGNORE
	sudo mount "${IMAGE_DIR}/a9rootfs.ext3" "${IMAGE_DIR}/rootfs" -t ext3 -o loop -o sync

exit:.IGNORE
	sudo umount "${IMAGE_DIR}/rootfs"

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o  *.cmd *.ko *.mod .tmp_versions *.order *.symvers *.mod.c .[a-zA-Z0-9-_]*

qemu:
	#sudo qemu-system-arm -M ${MACHINA} -m ${MEM} -kernel ${KERNEL} -dtb ${DTB} -nographic -append ${APPEND} -sd ${SD_ROOTFS} -nic tap,id=net0,ifname=${TAP}
	sudo qemu-system-arm -M ${MACHINA} -m ${MEM} -kernel ${KERNEL} -dtb ${DTB} -nographic -append ${APPEND} -sd ${SD_ROOTFS} ${NET_OPT}

MODs=$(shell ls|grep ".ko")
install:.IGNORE
ifeq ($(NET), 1)
	for MOD in ${MODs} ;\
	do \
		${TFTP_CMD} ${MACHINA_IP} -p -l "$${MOD}" ;\
	done
else
	for MOD in ${MODs} ;\
	do \
		sudo cp "$${MOD}" "${IMAGE_DIR}/rootfs/root" ;\
	done
endif

help:.IGNORE
	@echo "Help to work with this Makefile:"
	@echo -e "\tmake init			mount rootfs to host"
	@echo -e "\tmake exit			umount rootfs to host"
	@echo -e "\tmake net-init		init the net envirement to communicat with the qemu"
	@echo -e "\tmake net-exit		clean the net envirement"
	@echo -e "\tmake qemu			run the qemu"
	@echo -e "\tmake qemu NET=1		run the qemu in net modules"
	@echo -e "\tmake all				compile the kernel modules"
	@echo -e "\tmake clean			clean the compile envirement"
	@echo -e "\tmake install			copy the kernel modules to the rootfs"
	@echo -e "\tmake install NET=1	upload the kernel modules by atftp"

.IGNORE:
