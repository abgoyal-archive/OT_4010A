LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	src/pvasfffrec_factory.cpp \
 	src/pvasfffrec_plugin.cpp


LOCAL_MODULE := libpvasfffrecognizer

LOCAL_CFLAGS :=  $(PV_CFLAGS)



LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/pvmi/recognizer/plugins/pvasfffrecognizer/src \
 	$(PV_TOP)/pvmi/recognizer/plugins/pvasfffrecognizer/include \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
 	include/pvasfffrec_factory.h

include $(BUILD_STATIC_LIBRARY)
