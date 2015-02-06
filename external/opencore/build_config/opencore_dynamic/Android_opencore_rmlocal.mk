LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libpvrminterface \
 	libpvrmffparsernode

LOCAL_MODULE := libopencore_rmlocal

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libopencore_common libopencore_player

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/modules/linux_rm/core/Android.mk
include   $(PV_TOP)/nodes/pvmfrmffparsernode/Android.mk

