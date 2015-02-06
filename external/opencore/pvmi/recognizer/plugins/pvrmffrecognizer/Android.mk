LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvrmffrec_factory.cpp \
 	src/pvrmffrec_plugin.cpp


LOCAL_MODULE := libpvrmffrecognizer

LOCAL_CFLAGS :=  $(PV_CFLAGS)



LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/pvmi/recognizer/plugins/pvrmffrecognizer/src \
 	$(PV_TOP)/pvmi/recognizer/plugins/pvrmffrecognizer/include \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
 	include/pvrmffrec_factory.h

include $(BUILD_STATIC_LIBRARY)
