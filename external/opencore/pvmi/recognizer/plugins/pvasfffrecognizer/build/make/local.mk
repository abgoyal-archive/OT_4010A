LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := pvasfffrecognizer


XCXXFLAGS += $(FLAG_COMPILE_WARNINGS_AS_ERRORS)






SRCDIR := ../../src
INCSRCDIR := ../../include

SRCS := pvasfffrec_factory.cpp \
       pvasfffrec_plugin.cpp

HDRS := pvasfffrec_factory.h
    



include $(MK)/library.mk
