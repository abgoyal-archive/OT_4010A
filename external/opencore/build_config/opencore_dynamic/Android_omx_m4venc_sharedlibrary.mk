LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_m4venc_component_lib \
 	libpvm4vencoder

LOCAL_MODULE := libomx_m4venc_sharedlibrary

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common 

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)  	
LOCAL_SHARED_LIBRARIES += libmhal
endif

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/codecs_v2/omx/omx_m4venc/Android.mk
include   $(PV_TOP)/codecs_v2/video/m4v_h263/enc/Android.mk

