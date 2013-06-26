exfat-nofuse
============

Linux non-fuse read/write kernel driver for the exFAT file system.

Tested and works against kernel versions 3.8 and 3.9.

Kudos to github user ksv1986 for the 3.9 patch!


Installation:
> make

> make install

To load the driver manually, run this as root:
> modprobe exfat_fs


Free software for the free minds!
