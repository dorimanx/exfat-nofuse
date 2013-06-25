exfat-nofuse
============

Linux non-fuse read/write kernel driver for the exFat file system.

Tested and works against kernel version 3.8.11. Versions after 3.9.0 give an error about mutex_lock, 
I hope someone will fix it later.

Installation:
> make

> make install

To load the driver manually, run this as root:
> modprobe exfat_fs


Free software for the free minds!
