#########################################################
#
#	Telechips Linuxtv Demux OpenMax Component Library Make File 
#
#########################################################

#########################################################
#	Telechips Make File Initialize
#########################################################
# library/libpng
TOP := ../../

#########################################################
#	Read Telechips Linux Configuration File 
#########################################################
include $(TOP)config

#########################################################
#	Read Telechips Linux make rules File 
#########################################################
include $(TOP)rule.mk

#########################################################
#	Common Definitions
#########################################################
include $(TOP)common.mk

#########################################################
#	Common Path Include   
#########################################################
include $(BUILD_SYSTEM)basic_path.mk

#########################################################
#	Read  define file  
#########################################################
include	$(BUILD_SYSTEM)definition.mk

#########################################################
#	Reset Variable  
#########################################################
include $(RESET_VARS)

#########################################################
#	Version Set  
#########################################################
LOCAL_VERSION_MAJOR:=00
LOCAL_VERSION_MINOR:=01
LOCAL_VERSION := _$(LOCAL_VERSION_MAJOR).$(LOCAL_VERSION_MINOR)

#########################################################
#	Set Current Folder and Global Path 
#########################################################
LOCAL_PATH	:=./
LOCAL_SRC_PATH	:=./

#########################################################
#	Setting Target Folder 
#########################################################
TARGET_FOLDER := ./


#########################################################
#	Setting  Target Name 
#########################################################
STATIC	:=.a
SHARED	:=.so
LOCAL_TARGET := libpng


#########################################################
#	Add  Src File 
#########################################################
LOCAL_SRC_FILES := 

LOCAL_SRC_FILES += png.c
LOCAL_SRC_FILES += pngerror.c
LOCAL_SRC_FILES += pnggccrd.c
LOCAL_SRC_FILES += pngget.c
LOCAL_SRC_FILES += pngmem.c
LOCAL_SRC_FILES += pngpread.c
LOCAL_SRC_FILES += pngread.c
LOCAL_SRC_FILES += pngrio.c
LOCAL_SRC_FILES += pngrtran.c
LOCAL_SRC_FILES += pngrutil.c
LOCAL_SRC_FILES += pngset.c
LOCAL_SRC_FILES += pngtrans.c
LOCAL_SRC_FILES += pngvcrd.c
LOCAL_SRC_FILES += pngwio.c
LOCAL_SRC_FILES += pngwrite.c
LOCAL_SRC_FILES += pngwtran.c
LOCAL_SRC_FILES += pngwutil.c

#########################################################
#	Add  Header Path 
#########################################################
LOCAL_INCLUDE_HEADERS := 

LOCAL_INCLUDE_HEADERS += ../zlib
#LOCAL_INCLUDE_HEADERS += $(LOCAL_PATH)include


#########################################################
#	Add  local cflag  
#########################################################
LOCAL_CFLAGS:=

LOCAL_CFLAGS += -fPIC
LOCAL_CFLAGS += -std=gnu89 -fvisibility=hidden ## -fomit-frame-pointer

#########################################################
#	Add  shared lib  
#########################################################
LOCAL_SHARED_LIB :=


#########################################################
#	Add  copy lib  
#########################################################
LOCAL_COPY_LIB :=


#########################################################
#	Add  static lib  
#########################################################
LOCAL_STATIC_LIB :=


#########################################################
#	Add  static lib  
#########################################################
COPY_HEADER :=
COPY_HEADER += png.h pngconf.h pngusr.h


include	$(BUILD_SYSTEM)build_object.mk

#########################################################
#	Make All Function 
#########################################################
all :
	$(call build-clean-obj)
#	$(call build-lib-static)
	$(call build-lib-shared)

#########################################################
#	Make Install Function 
#########################################################
install : 
	$(MAKE) all 
#ifdef 	LOCAL_COPY_LIB	
#	$(call build-copy-src-to-dest,$(LOCAL_COPY_LIB),$(COPY_TO_LIB_PATH))
#endif
#ifdef 	COPY_HEADER
#	$(call build-copy-src-to-dest,$(COPY_HEADER),$(SYSTEM_INCLUDE))
#endif
#	$(call build-copy-to-lib,$(LOCAL_TARGET)$(STATIC),$(COPY_TO_LIB_PATH))
	$(call build-copy-to-lib,$(LOCAL_TARGET)$(SHARED),$(SYSTEM_LIB))


#########################################################
#	Make Clean Function 
#########################################################	
clean : 
	$(call build-clean-obj)


#########################################################
#	Make Debug Function 
#########################################################	
debug : 
	$(call build-debug-print)
