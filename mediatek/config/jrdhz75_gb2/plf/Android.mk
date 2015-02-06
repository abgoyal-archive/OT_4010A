LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

KEYPADLAYOUT_PLF := $(TOPDIR)mediatek/config/jrdhz75_gb2/plf/isdm_KeypadLayout.plf

KEYPADLAYOUT_KL := $(TOPDIR)out/target/product/jrdhz75_gb2/custpack/usr/keylayout/isdm_KeypadLayout.kl

$(KEYPADLAYOUT_KL) : $(KEYPADLAYOUT_PLF)
	$(hide) mkdir -p $(JRD_OUT_CUSTPACK)/usr/keylayout
	$(TOPDIR)development/jrdtools/arct/prebuilt/arct \
       k \
	$(KEYPADLAYOUT_PLF) \
	$(KEYPADLAYOUT_KL)

ALL_PREBUILT += $(KEYPADLAYOUT_KL)


