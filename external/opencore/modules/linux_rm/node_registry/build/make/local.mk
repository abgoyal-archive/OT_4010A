LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := pvmtkrawsourcereginterface

SRCDIR := ../../src
INCSRCDIR := ../../src

SRCS := pvmtkrawsourcenodereg.cpp

HDRS := pvmtkrawsourcenodereg.h

include $(MK)/library.mk

