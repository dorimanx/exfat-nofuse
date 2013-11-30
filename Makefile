#
# Makefile for Linux FAT12/FAT16/FAT32(VFAT)/FAT64(ExFAT) filesystem driver.
#

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-$(CONFIG_EXFAT_FS) += exfat.o

exfat-objs := exfat_core.o exfat_super.o exfat_api.o exfat_blkdev.o exfat_cache.o \
			   exfat_data.o exfat_bitmap.o exfat_nls.o exfat_oal.o exfat_upcase.o

else
# external module build

EXTRA_FLAGS += -I$(PWD)

#KDIR	:= /usr/src/linux/
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

export CONFIG_EXFAT_FS := m

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

help:
	$(MAKE) -C $(KDIR) M=$(PWD) help

install: exfat.ko
	rm -f ${KDIR}/../kernel/fs/exfat/exfat.ko
	install -m644 -b -D exfat.ko ${KDIR}/../kernel/fs/exfat/exfat.ko
	depmod -aq

uninstall:
	rm -rf ${KDIR}/../kernel/fs/exfat
	depmod -aq

endif

.PHONY : all clean install uninstall 
