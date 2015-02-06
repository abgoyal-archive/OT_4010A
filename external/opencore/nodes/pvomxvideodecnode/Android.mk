LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/mpeg4_hardware.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/avc_hardware.mk \
)

LOCAL_SRC_FILES := \
	src/pvmf_omx_videodec_factory.cpp \
 	src/pvmf_omx_videodec_node.cpp


LOCAL_MODULE := libpvomxvideodecnode

LOCAL_CFLAGS :=  $(PV_CFLAGS)

ifeq ($(strip $(BOARD_USES_6573_MFV_HW)),true)
  LOCAL_CFLAGS += -DMT6573_MFV_HW
endif

ifeq ($(strip $(BOARD_USES_YUSU_VIDEO_DECODER_OUTPUT_FORMAT)),true)
  LOCAL_CFLAGS += -DMT6516_VIDEO_DECODER_OUTPUT_FORMAT
endif

ifeq ($(strip $(BOARD_USES_YUSU_MPEG4_HW_DECODER_V2)),true)
  LOCAL_CFLAGS += -DMT6516_MP4_HW_DECODER_V2
endif

ifeq ($(strip $(BOARD_USES_YUSU_AVC_USE_EXT_BUF)),true)
  LOCAL_CFLAGS += -DAVC_USE_EXT_BUF
endif

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/nodes/pvomxvideodecnode/src \
 	$(PV_TOP)/nodes/pvomxvideodecnode/include \
 	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
 	$(PV_TOP)/codecs_v2/video/wmv_vc1/dec/src \
 	$(PV_TOP)/baselibs/threadsafe_callback_ao/src \
 	$(PV_TOP)/nodes/pvomxbasedecnode/include \
 	$(PV_TOP)/nodes/pvomxbasedecnode/src \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
 	include/pvmf_omx_videodec_factory.h

include $(BUILD_STATIC_LIBRARY)
