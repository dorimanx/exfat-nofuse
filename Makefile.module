# Exfat Linux kernel modules
#

obj-m += exfat.o

exfat-y := exfat_core.o exfat_api.o exfat_blkdev.o exfat_cache.o exfat_super.o \
   		   exfat_data.o exfat_global.o exfat_nls.o exfat_oal.o exfat_upcase.o

EXTRA_FLAGS += -I$(PWD)

#KDIR	:= /usr/src/linux/
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

help:
	$(MAKE) -C $(KDIR) M=$(PWD) help

.PHONY : install
install : all
	sudo $(MAKE) -C $(KDIR) M=$(PWD) modules_install; sudo depmod
