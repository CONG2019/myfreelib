MACHINA="vexpress-a9"

DTB="/home/cong/work/qemu/vexpress-a9/vexpress-v2p-ca9.dtb"
KERNEL="/home/cong/work/qemu/vexpress-a9/zImage"
SD_ROOTFS="/home/cong/work/qemu/vexpress-a9/a9rootfs.ext3"

MEM="512M"
APPEND="root=/dev/mmcblk0 rw console=ttyAMA0"

TAP="tap0"
BRIDGE="br0"
HOST_IP="192.168.1.1"
MACHINA_IP="192.168.1.2"

net-init:.IGNORE
	sudo brctl addbr ${BRIDGE}
	sudo ifconfig ${BRIDGE} up
	sudo ifconfig ${BRIDGE} ${HOST_IP}

	sudo tunctl -t ${TAP} -u root
	sudo brctl addif ${BRIDGE} ${TAP}
	sudo ifconfig ${TAP} 0.0.0.0 promisc up

net-clean:.IGNORE
	sudo ip link delete ${TAP} > /dev/null
	sudo ifconfig ${BRIDGE} down
	sudo brctl delbr ${BRIDGE}

qemu:
	sudo qemu-system-arm -M ${MACHINA} -m ${MEM} -kernel ${KERNEL} -dtb ${DTB} -nographic -append ${APPEND} -sd ${SD_ROOTFS} -nic tap,id=net0,ifname=${TAP}

upload:.IGNORE
	for MOD in `ls |grep ".ko"` \
	do \
		tftp ${MACHINA_IP} -p -l ${MOD} \
	done

.IGNORE:
