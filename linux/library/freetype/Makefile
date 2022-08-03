#########################################################
#
#	Telechips Linuxtv Demux OpenMax Component Library Make File 
#
#########################################################

#########################################################
#	Telechips Make File Initialize
#########################################################
# system/freetype
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
LOCAL_SRC_PATH	:=./src/

#########################################################
#	Setting Target Folder 
#########################################################
TARGET_FOLDER := ./


#########################################################
#	Setting  Target Name 
#########################################################
STATIC	:=.a
SHARED	:=.so
LOCAL_TARGET := libft2


#########################################################
#	Add  Src File 
#########################################################
LOCAL_SRC_FILES := 

LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftbbox.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftbitmap.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftglyph.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftstroke.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftxf86.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftbase.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftsystem.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftinit.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)base/ftgasp.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)raster/raster.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)sfnt/sfnt.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)smooth/smooth.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)autofit/autofit.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)truetype/truetype.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)cff/cff.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)psnames/psnames.c
LOCAL_SRC_FILES += $(LOCAL_SRC_PATH)pshinter/pshinter.c


#########################################################
#	Add  Header Path 
#########################################################
LOCAL_INCLUDE_HEADERS := 

LOCAL_INCLUDE_HEADERS += $(LOCAL_PATH)include
LOCAL_INCLUDE_HEADERS += ../libpng
LOCAL_INCLUDE_HEADERS += ../zlib


#########################################################
#	Add  local cflag  
#########################################################
LOCAL_CFLAGS:=

LOCAL_CFLAGS += -fPIC
LOCAL_CFLAGS += -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"
#LOCAL_CFLAGS += "-DTT_CONFIG_OPTION_BYTECODE_INTERPRETER"

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

LOCAL_STATIC_LIB += $(SYSTEM_LIB)libpng.so
LOCAL_STATIC_LIB += $(SYSTEM_LIB)libz.so


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
ifdef 	LOCAL_COPY_LIB	
	$(call build-copy-src-to-dest,$(LOCAL_COPY_LIB),$(COPY_TO_LIB_PATH))
endif
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
