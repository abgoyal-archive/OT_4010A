LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/mpeg4_hardware.mk \
)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/oscl_pmem.mk \
)

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
  LOCAL_CFLAGS += -DMHAL_H_INCLUDED 
endif

LOCAL_SRC_FILES := \
    autodetect.cpp \
    metadatadriver.cpp \
    playerdriver.cpp \
    thread_init.cpp \
    pvmediascanner.cpp \
    android_surface_output.cpp \
    android_audio_output.cpp \
    android_audio_stream.cpp \
    android_audio_mio.cpp \
    android_audio_output_threadsafe_callbacks.cpp

#LOCAL_CFLAGS := $(PV_CFLAGS)

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

ifeq ($(strip $(BOARD_USES_PMEM_NON_CACHED)),true)
  LOCAL_CFLAGS += -DPMEM_NON_CACHED
endif

LOCAL_C_INCLUDES := $(PV_INCLUDES) \
    $(PV_TOP)/engines/common/include \
    $(PV_TOP)/fileformats/mp4/parser/include \
    $(PV_TOP)/pvmi/media_io/pvmiofileoutput/include \
    $(PV_TOP)/nodes/pvmediaoutputnode/include \
    $(PV_TOP)/nodes/pvmediainputnode/include \
    $(PV_TOP)/nodes/pvmp4ffcomposernode/include \
    $(PV_TOP)/engines/player/include \
    $(PV_TOP)/nodes/common/include \
    $(PV_TOP)/fileformats/pvx/parser/include \
	$(PV_TOP)/nodes/pvprotocolenginenode/download_protocols/common/src \
    libs/drm/mobile1/include \
    include/graphics \
    external/skia/include/corecg \
    $(call include-path-for, graphics corecg) \
    $(MTK_PATH_SOURCE)/external/vorbisdec/Tremor   \
    external/icu4c/common \
    

LOCAL_MODULE := libandroidpv

LOCAL_SHARED_LIBRARIES := libui libutils libbinder libsurfaceflinger_client libcamera_client
LOCAL_STATIC_LIBRARIES := libosclbase libosclerror libosclmemory libosclutil

LOCAL_LDLIBS +=

include $(BUILD_STATIC_LIBRARY)

