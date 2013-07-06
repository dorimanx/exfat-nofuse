exfat-nofuse
============

Linux non-fuse read/write kernel driver for the exFAT file system.<br />
Originally ported from android kernel v3.0.


Tested and works against linux kernel versions 3.8, 3.9 and 3.10.<br />
Kudos to github user ksv1986 for the 3.9 compatibility patch!


Installation:
> make<br />
> make install

To load the driver manually, run this as root:
> modprobe exfat_fs


Free software for the free minds!
