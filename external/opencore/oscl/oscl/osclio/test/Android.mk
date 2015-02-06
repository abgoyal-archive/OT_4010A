LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
 	src/test_case_file_io.cpp \
 	src/test_oscl_io.cpp


LOCAL_MODULE := oscl_io_test

LOCAL_CFLAGS := $(PV_CFLAGS)



LOCAL_STATIC_LIBRARIES := libunit_test  

LOCAL_SHARED_LIBRARIES := libopencore_common

LOCAL_C_INCLUDES := \
	$(PV_TOP)/oscl/oscl/osclio/test/src \
	$(PV_TOP)/fileformats/common/parser/include \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
 	

-include $(PV_TOP)/Android_system_extras.mk

include $(BUILD_EXECUTABLE)
