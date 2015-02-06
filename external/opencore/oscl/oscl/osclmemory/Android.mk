LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/oscl_pmem.mk \
)

LOCAL_SRC_FILES := \
        src/oscl_mem.cpp \
        src/oscl_mem_imp.cpp \
        src/oscl_mem_audit.cpp \
        src/oscl_mem_mempool.cpp \
        src/oscl_pmem.cpp 


LOCAL_MODULE := libosclmemory

LOCAL_CFLAGS :=  $(PV_CFLAGS)

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_CFLAGS += -DMT6573_MFV_HW
endif

ifeq ($(strip $(BOARD_USES_PMEM_IN_OSCL)),true)
  LOCAL_CFLAGS += -DUSES_PMEM_IN_OSCL
endif


LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
        $(PV_TOP)/oscl/oscl/osclmemory/src \
        $(PV_TOP)/oscl/oscl/osclmemory/src \
        $(PV_INCLUDES)

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
LOCAL_C_INCLUDES += $(MTK_PATH_PLATFORM)/kernel/drivers/mflexvideo
endif

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
        src/oscl_mem.h \
        src/oscl_mem_audit.h \
        src/oscl_mem_auto_ptr.h \
        src/oscl_mem_mempool.h \
        src/oscl_mem.inl \
        src/oscl_pmem.h

include $(BUILD_STATIC_LIBRARY)
