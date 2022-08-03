obj-m := mn88472_fe.o

mn88472_fe-objs := mn88472.o

mn88472_fe-objs += mn88472/MN_DMD_device.o \
                   mn88472/MN88472_register.o \
                   mn88472/MN88472_autoctrl.o \
                   tuner_template/MN_Tuner.o \
                   tuner_template/MxL603_Main.o \
                   tuner_template/MxL603_TunerApi.o \
                   tuner_template/MxL603_OEM_Drv.o \
                   tuner_template/MxL603_TunerCfg.o \
                   common/MN_DMD_driver.o \
                   common/MN_DMD_common.o \
                   i2c_template/MN_I2C.o \
                   mn88472_sample/MN88472_UserFunction.o

EXTRA_CFLAGS := -Idrivers/spi/tcc_dxb_drv \
                -Idrivers/media/dvb-core \
                -I$(src)/mn88472 \
                -I$(src)/common \
                -I$(src)/tuner_template

KDIR ?= $(ANDROID_BUILD_TOP)/kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
