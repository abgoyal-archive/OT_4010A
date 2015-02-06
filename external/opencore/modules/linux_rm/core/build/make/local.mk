LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := mtkrawsourceinterface

SRCDIR := ../../src

SRCS := pvmfmtkrawsourcenodes.cpp

include $(MK)/library.mk

