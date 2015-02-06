LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libpvmp4ffcomposer \
 	libpvmp4ffcomposernode \
 	libpvauthorengine

LOCAL_WHOLE_STATIC_LIBRARIES +=  libandroidpvauthor

# to solve circular dependency among the static libraries.
LOCAL_STATIC_LIBRARIES := $(LOCAL_STATIC_LIBRARIES) $(LOCAL_WHOLE_STATIC_LIBRARIES)

LOCAL_MODULE := libopencore_author

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES += libbinder libopencore_common libcameraservice libcamera_client

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true) 
LOCAL_SHARED_LIBRARIES += libmhal 
endif

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/fileformats/mp4/composer/Android.mk
include   $(PV_TOP)/nodes/pvmp4ffcomposernode/Android.mk
include   $(PV_TOP)/engines/author/Android.mk
include $(PV_TOP)/android/author/Android.mk

