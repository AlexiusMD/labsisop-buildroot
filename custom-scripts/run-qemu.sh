#!/bin/sh

qemu-system-i386 --kernel output/images/bzImage --hda output/images/rootfs.ext2 --nographic --append "console=ttyS0 root=/dev/sda"
