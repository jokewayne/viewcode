obj-m += sink.o
obj-m += sink_symb.o
obj-m += sink_param.o
obj-m += sink_chat.o
CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL_PATH:=/home/pi/linux-rpi-4.4.y
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean

sink_symb-objs += sink_symb_b.o sink_symb_a.o


