#!/bin/bash
BUSYBOX_PATH=/home/conguyen/repos/personal/busybox-1.36.1
CURRENT_DIR=$(pwd)

mkdir -p rootfs/{bin,sbin,etc,proc,sys,usr/{bin,sbin},dev,lib,var/{log,run}}
cd rootfs

sudo mknod -m 660 dev/mem c 1 1
sudo mknod -m 660 dev/tty2 c 4 2
sudo mknod -m 660 dev/tty3 c 4 3
sudo mknod -m 660 dev/tty4 c 4 4
sudo mknod -m 660 dev/null c 1 3
sudo mknod -m 660 dev/zero c 1 5

cp -av ${BUSYBOX_PATH}/_install/* .
ln -sf bin/busybox init

mkdir -p etc/init.d/
cat > etc/init.d/rcS << EOF0
mount -t sysfs none /sys
mount -t proc none /proc
EOF0

chmod -R 777 etc/init.d/rcS

find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../rootfs.cpio.gz
cd -
