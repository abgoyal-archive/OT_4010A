LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/mpeg4_hardware.mk \
)

LOCAL_SRC_FILES := \
	src/pvmf_omx_basedec_node.cpp \
 	src/pvmf_omx_basedec_port.cpp \
 	src/pvmf_omx_basedec_callbacks.cpp


LOCAL_MODULE := libpvomxbasedecnode

LOCAL_CFLAGS :=  $(PV_CFLAGS)

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_CFLAGS += -DMT6573_MFV_HW
endif

ifeq ($(strip $(BOARD_USES_YUSU_VIDEO_DECODER_OUTPUT_FORMAT)),true)
  LOCAL_CFLAGS += -DMT6516_VIDEO_DECODER_OUTPUT_FORMAT
endif

ifeq ($(strip $(BOARD_USES_YUSU_OMX_PMEM)),true)
  LOCAL_CFLAGS += -DMT6516_OMX_USE_PMEM
endif

ifeq ($(strip $(BOARD_USES_YUSU_MPEG4_HW_DECODER_V2)),true)
  LOCAL_CFLAGS += -DMT6516_MP4_HW_DECODER_V2
endif

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/nodes/pvomxbasedecnode/src \
 	$(PV_TOP)/nodes/pvomxbasedecnode/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
	include/pvmf_omx_basedec_defs.h \
 	include/pvmf_omx_basedec_port.h \
 	include/pvmf_omx_basedec_node.h \
 	include/pvmf_omx_basedec_callbacks.h \
 	include/pvmf_omx_basedec_node_extension_interface.h

include $(BUILD_STATIC_LIBRARY)
