LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := pvmtkrawsourcenode




XINCDIRS += ../../src/default 
XINCDIRS += ../../../../fileformats/mp4/parser/utils/mp4recognizer/include




SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := pvmf_mtkrawsource_node.cpp pvmf_mtkrawsource_factory.cpp pvmf_mtkrawsource_outport.cpp pvmf_mtkrawsource_node_metadata.cpp pvmf_mtkrawsource_node_cap_config.cpp

HDRS := pvmf_mtkrawsource_factory.h \
	pvmf_mtkrawsource_events.h



include $(MK)/library.mk
