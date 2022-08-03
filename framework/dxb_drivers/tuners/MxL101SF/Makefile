obj-m := mxl101sf_fe.o 

mxl101sf_fe-objs := mxl101sf.o

mxl101sf_fe-objs += src/demod_MxL101SF.o \
                    src/MxL101SF_OEM_Drv.o \
                    src/MxL101SF_PhyCfg.o \
                    src/MxL101SF_PhyCtrlApi.o

EXTRA_CFLAGS := -Idrivers/spi/tcc_dxb_drv \
                -Idrivers/media/dvb-core

KDIR ?= $(ANDROID_BUILD_TOP)/kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
