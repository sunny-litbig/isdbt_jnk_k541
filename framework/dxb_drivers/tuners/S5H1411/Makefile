obj-m := s5h1411_fe.o 

s5h1411_fe-objs := s5h1411.o

EXTRA_CFLAGS := -Idrivers/spi/tcc_dxb_drv \
                -Idrivers/media/dvb-core \
                -Idrivers/media/tuners \
                -Idrivers/media/video

KDIR ?= $(ANDROID_BUILD_TOP)/kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
