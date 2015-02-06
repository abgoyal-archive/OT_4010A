LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := pvmtkffrecognizer


XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)






SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := pvmtkffrec_factory.cpp \
       pvmtkffrec_plugin.cpp

HDRS := pvmtkffrec_factory.h
    



include $(MK)/library.mk
