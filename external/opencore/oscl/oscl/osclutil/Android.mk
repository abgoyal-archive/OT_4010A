LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

LOCAL_SRC_FILES := \
        src/oscl_bin_stream.cpp \
        src/oscl_string.cpp \
        src/oscl_string_containers.cpp \
        src/oscl_str_ptr_len.cpp \
        src/oscl_string_utf8.cpp \
        src/oscl_tickcount.cpp \
        src/oscl_utf8conv.cpp \
        src/oscl_snprintf.cpp \
        src/oscl_math.cpp \
        src/oscl_rand.cpp \
        src/oscl_string_uri.cpp \
        src/oscl_string_xml.cpp \
        src/oscl_priqueue.cpp \
        src/oscl_mm_queue.cpp \
        src/oscl_profile.cpp \
        src/oscl_file_cfg.cpp \
        src/oscl_trace.cpp


LOCAL_MODULE := libosclutil

LOCAL_CFLAGS :=  $(PV_CFLAGS)

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
  LOCAL_CFLAGS += -DMHAL_H_INCLUDED
endif

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
        $(PV_TOP)/oscl/oscl/osclutil/src \
        $(PV_TOP)/oscl/oscl/osclutil/src \
        $(LOCAL_PATH) $(MTK_PATH_PLATFORM)/kernel/core/include/mach \

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
LOCAL_C_INCLUDES += \
        $(LOCAL_PATH) $(MTK_PATH_SOURCE)/external/mhal/inc \
        $(LOCAL_PATH) $(MTK_PATH_SOURCE)/external/mhal/src/lib/libutility 
endif

LOCAL_C_INCLUDES += $(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
        src/oscl_bin_stream.h \
        src/oscl_bin_stream.inl \
        src/oscl_media_data.h \
        src/oscl_media_status.h \
        src/oscl_priqueue.h \
        src/oscl_rand.h \
        src/oscl_rand.inl \
        src/oscl_snprintf.h \
        src/oscl_string.h \
        src/oscl_string_containers.h \
        src/oscl_string_rep.h \
        src/oscl_string_utf8.h \
        src/oscl_str_ptr_len.h \
        src/oscl_tickcount.h \
        src/oscl_tickcount.inl \
        src/oscl_tickcount.cpp \
        src/oscl_utf8conv.h \
        src/oscl_math.h \
        src/oscl_math.inl \
        src/oscl_string_uri.h \
        src/oscl_string_xml.h \
        src/oscl_mm_queue.h \
        src/oscl_profile.h \
        src/oscl_file_cfg.h \
        src/oscl_trace.h

include $(BUILD_STATIC_LIBRARY)
