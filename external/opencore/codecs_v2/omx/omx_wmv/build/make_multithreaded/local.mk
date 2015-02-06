LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := omx_wmv_component_lib

XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)

OPTIMIZE_FOR_PERFORMANCE_OVER_SIZE := true

XINCDIRS += \
        ../../../../../extern_libs_v2/khronos/openmax/include \
        ../../../../omx/omx_baseclass/include

SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS :=	omx_wmv_component.cpp \
	wmv_dec.cpp 


HDRS :=	omx_wmv_component.h \
	wmv_dec.h \
	wmvvideo_frame.h



include $(MK)/library.mk

