LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := exploit 
LOCAL_SRC_FILES := exploit.c
LOCAL_LDFLAGS += -static
include $(BUILD_EXECUTABLE)
