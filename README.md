exfat-nofuse
============

Linux non-fuse read/write kernel driver for the exFAT file system.<br />
Originally ported from android kernel v3.0.


Kudos to ksv1986 for the mutex patch!<br />
Thanks to JackNorris for being awesome and providing the clear_inode() patch.<br />
<br />
Big thanks to lqs for completing the driver!


Special thanks to github user AndreiLux for spreading the word about the leak!<br />

To load the driver manually, run this as root:
> modprobe exfat


Free Software for the Free Minds!
=====================================

To add to kernel you need to do this:

cd your kernel source dir

mkdir fs/exfat

copy all files (exept .git) from exfat-nofuse to your kernel source fs/exfat/

see
https://github.com/dorimanx/Dorimanx-SG2-I9100-Kernel/commit/e8fc728a68096db9ffcebff40244ebfb60a3de18

edit fs/Kconfig
edit fs/Makefile

cd your kernel source
make menuconfig

go to: File systems > DOS/FAT/NT > check the exfat as MODULE (M)
<M> exFAT filesystem support
(437) Default codepage for exFAT
(utf8) Default iocharset for exFAT

ESC to main menu
Save an Alternate Configuration File
ESC ESC

build your kernel.

and you will have new module!

exfat.ko

have fun.