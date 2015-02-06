LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call make-private-dependency,\
  $(BOARD_CONFIG_DIR)/configs/openCore.mk \
)

# don't export mio symbols
LOCAL_CFLAGS += -DHIDE_MIO_SYMBOLS

# Temporary workaround
ifeq ($(strip $(USE_SHOLES_PROPERTY)),true)
LOCAL_CFLAGS += -DSHOLES_PROPERTY_OVERRIDES
endif

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)
  LOCAL_CFLAGS += -DMHAL_H_INCLUDED 
endif

LOCAL_SRC_FILES := \
    authordriver.cpp \
    PVMediaRecorder.cpp \
    android_camera_input.cpp \
    android_audio_input.cpp \
    android_audio_input_threadsafe_callbacks.cpp \
    ../thread_init.cpp \

LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := $(PV_INCLUDES) \
    $(PV_TOP)/engines/common/include \
    $(PV_TOP)/codecs_v2/omx/omx_common/include \
    $(PV_TOP)/fileformats/mp4/parser/include \
    $(PV_TOP)/pvmi/media_io/pvmiofileoutput/include \
    $(PV_TOP)/nodes/pvmediaoutputnode/include \
    $(PV_TOP)/nodes/pvmediainputnode/include \
    $(PV_TOP)/nodes/pvmp4ffcomposernode/include \
    $(PV_TOP)/engines/player/include \
    $(PV_TOP)/nodes/common/include \
    $(call include-path-for, graphics corecg) \
    $(MTK_PATH_SOURCE)/external/vorbisdec/Tremor \
    libs/drm/mobile1/include 
    
ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)     
LOCAL_C_INCLUDES += \
    $(MTK_PATH_SOURCE)/external/mhal/inc
endif

LOCAL_SHARED_LIBRARIES := libmedia libbinder libcameraservice libcamera_client

ifeq ($(strip $(BOARD_USES_YUSU_HW)),true)  
LOCAL_SHARED_LIBRARIES += libmhal 
endif

LOCAL_MODULE := libandroidpvauthor

LOCAL_LDLIBS += 

include $(BUILD_STATIC_LIBRARY)

