LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/wmv_hardware.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/oscl_pmem.mk \
)

LOCAL_SRC_FILES := \
	src/omx_wmv_component.cpp \
 	src/wmv_dec.cpp

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_CFLAGS += -DMT6573_MFV_HW
endif


ifeq ($(strip $(BOARD_USES_YUSU_OMX_PROFILING)),true)
  LOCAL_CFLAGS += -DMT6516_MP4DEC_OMX_PROFILING
endif

ifeq ($(strip $(BOARD_USES_PMEM_NON_CACHED)),true)
  LOCAL_CFLAGS += -DPMEM_NON_CACHED
endif

LOCAL_MODULE := libomx_wmv_component_lib

LOCAL_CFLAGS +=  $(PV_CFLAGS)

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
  LOCAL_CFLAGS += -DMHAL_H_INCLUDED
endif

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_SHARED_LIBRARIES += libmfv_common
endif

LOCAL_C_INCLUDES := \
  $(PV_TOP)/android \
	$(PV_TOP)/codecs_v2/omx/omx_wmv/src \
 	$(PV_TOP)/codecs_v2/omx/omx_wmv/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
 	$(PV_TOP)/codecs_v2/omx/omx_baseclass/include \
 	$(PV_TOP)/oscl/oscl/osclmemory/src 


ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
LOCAL_C_INCLUDES += $(MTK_PATH_SOURCE)/kernel/include/linux \
                    $(MTK_PATH_SOURCE)/external/mhal/src/core/drv/inc \
                    $(MTK_PATH_SOURCE)/external/emi/inc
endif

LOCAL_C_INCLUDES += $(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
	include/omx_wmv_component.h \
 	include/wmv_dec.h \
 	include/wmvvideo_frame.h

include $(BUILD_STATIC_LIBRARY)
