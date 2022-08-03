obj-m := avl6211_fe.o

avl6211_fe-objs := avl6211.o \
                   AVL6211_API.o

avl6211_fe-objs += src/avl.o \
                   src/avl_dvbsx.o \
                   src/ExtAV2011.o \
                   src/ExtSharpBS2S7HZ6306.o \
                   src/IBase.o \
                   src/IBlindScan.o \
                   src/IBlindscanAPI.o \
                   src/IBSP.o \
                   src/IDiseqc.o \
                   src/II2C.o \
                   src/II2CRepeater.o \
                   src/IRx.o \
                   src/ITuner.o \

EXTRA_CFLAGS := -Idrivers/spi/tcc_dxb_drv \
                -Idrivers/media/dvb-core

KDIR ?= $(ANDROID_BUILD_TOP)/kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
