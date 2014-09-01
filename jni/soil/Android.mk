# Android module for SOIL - to use it in your project:
# LOCAL_STATIC_LIBRARIES := soil
# $(call import-module,soil)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := soil
LOCAL_SRC_FILES := lib/libSOIL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)
