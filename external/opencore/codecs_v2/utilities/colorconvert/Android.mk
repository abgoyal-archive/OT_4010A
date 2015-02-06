LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/mpeg4_hardware.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/oscl_pmem.mk \
)

LOCAL_SRC_FILES := \
	src/ccrgb16toyuv420.cpp \
 	src/ccrgb24torgb16.cpp \
 	src/ccyuv422toyuv420.cpp \
 	src/cczoomrotation12.cpp \
 	src/cczoomrotation16.cpp \
 	src/cczoomrotation24.cpp \
 	src/cczoomrotation32.cpp \
 	src/cczoomrotationbase.cpp \
 	src/cpvvideoblend.cpp \
 	src/ccrgb24toyuv420.cpp \
 	src/ccrgb12toyuv420.cpp \
 	src/ccyuv420semiplnrtoyuv420plnr.cpp \
 	src/ccyuv420toyuv420semi.cpp

ifeq ($(strip $(BOARD_USES_YUSU_MPEG4_HW_DECODER_V2)),true)
  LOCAL_CFLAGS += -DMT6516_MP4_HW_DECODER_V2
endif

ifeq ($(strip $(BOARD_USES_PMEM_NON_CACHED)),true)
  LOCAL_CFLAGS += -DPMEM_NON_CACHED
endif

LOCAL_MODULE := libcolorconvert

LOCAL_CFLAGS += -DFALSE=false $(PV_CFLAGS)

# Temporary workaround
ifeq ($(strip $(USE_SHOLES_PROPERTY)),true)
  LOCAL_CFLAGS += -DSHOLES_PROPERTY_OVERRIDES
endif

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
  LOCAL_CFLAGS += -DMHAL_H_INCLUDED
endif

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/codecs_v2/utilities/colorconvert/src \
 	$(PV_TOP)/codecs_v2/utilities/colorconvert/include \
        
ifeq ($(strip $(BOARD_USES_YUSU_HW)),true) 	
  LOCAL_C_INCLUDES += $(LOCAL_PATH) $(MTK_PATH_SOURCE)/external/mhal/inc 
endif

LOCAL_C_INCLUDES += $(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
	include/cczoomrotationbase.h \
 	include/cczoomrotation12.h \
 	include/cczoomrotation16.h \
 	include/cczoomrotation24.h \
 	include/cczoomrotation32.h \
 	include/ccrgb16toyuv420.h \
 	include/ccrgb24torgb16.h \
 	include/ccyuv422toyuv420.h \
 	include/colorconv_config.h \
 	include/pvvideoblend.h \
 	include/ccrgb24toyuv420.h \
 	include/ccrgb12toyuv420.h \
 	include/ccyuv420semitoyuv420.h \
 	include/ccyuv420toyuv420semi.h

include $(BUILD_STATIC_LIBRARY)
