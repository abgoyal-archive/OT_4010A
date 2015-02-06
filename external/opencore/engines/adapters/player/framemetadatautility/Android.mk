LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/mpeg4_hardware.mk \
)

LOCAL_SRC_FILES := \
	src/pv_frame_metadata_utility.cpp \
 	src/pv_frame_metadata_factory.cpp \
 	src/pv_frame_metadata_mio_video.cpp \
 	src/pv_frame_metadata_mio_audio.cpp \
 	src/../config/common/pv_frame_metadata_mio_video_config.cpp


LOCAL_MODULE := libpvframemetadatautility

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

LOCAL_STATIC_LIBRARIES := 

LOCAL_SHARED_LIBRARIES := 

LOCAL_C_INCLUDES := \
	$(PV_TOP)/engines/adapters/player/framemetadatautility/src \
 	$(PV_TOP)/engines/adapters/player/framemetadatautility/include \
 	$(PV_TOP)/engines/adapters/player/framemetadatautility/config/android \
 	$(PV_INCLUDES)

LOCAL_COPY_HEADERS_TO := $(PV_COPY_HEADERS_TO)

LOCAL_COPY_HEADERS := \
	include/pv_frame_metadata_factory.h \
 	include/pv_frame_metadata_interface.h

include $(BUILD_STATIC_LIBRARY)
