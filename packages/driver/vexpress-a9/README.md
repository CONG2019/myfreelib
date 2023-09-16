# How to start

## work in mount mode
1. make init
2. Editing our driver
3. make all
4. make install
5. make qemu
6. The `ko` were copy to /root
7. insmod xxx.ko
8. rmmod xxx.ko
9. ctrl+a x : close the  qemu
10. make exit

## work in net mode
1. make net-init : will create the tap0 and br0, and host's IP is set to 192.168.99.1
2. Editing our driver
3. make all
4. make qemu NET=1
    - ifconfig eth0 up
    - ifconfig eth0 192.168.99.2 : the qemu machine's IP is set to 192.168.99.2
    - su
    - udpsvd -vE 0.0.0.0 69 tftpd -cl -u root /root& : start tftpd 
5. make install NET=1
6. The `ko` were upload to /root
7. insmod xxx.ko
8. rmmod xxx.ko
9. ctrl+a x : close the  qemu
10. make net-exit