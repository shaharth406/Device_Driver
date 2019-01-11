obj-m=simple_usb.o

KDIR=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clear:
	make -C $(KDIR) M=$(shell pwd) clean
