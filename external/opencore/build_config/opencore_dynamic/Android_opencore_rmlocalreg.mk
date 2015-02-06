LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := \
	libpvrmreginterface \
 	libpvrmffrecognizer

LOCAL_MODULE := libopencore_rmlocalreg

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

LOCAL_SHARED_LIBRARIES +=   libopencore_player libopencore_common

include $(BUILD_SHARED_LIBRARY)
include   $(PV_TOP)/modules/linux_rm/node_registry/Android.mk
include   $(PV_TOP)/pvmi/recognizer/plugins/pvrmffrecognizer/Android.mk

