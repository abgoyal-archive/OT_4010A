LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/rv_hardware.mk \
)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libomx_rv_component_lib

LOCAL_MODULE := libomx_rvdec_sharedlibrary

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libomx_sharedlibrary libopencore_common libstagefright_pmemutil

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_SHARED_LIBRARIES += libmfv_common
endif

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true) 
LOCAL_SHARED_LIBRARIES += libmhal
endif

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/codecs_v2/omx/omx_rv/Android.mk
