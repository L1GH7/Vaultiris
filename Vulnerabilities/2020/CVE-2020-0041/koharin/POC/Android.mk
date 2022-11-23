LOCAL_PATH := $(call my-dir)
LOCAL_MODULE := poc
LOCAL_SRC_FILES := src/poc.c src/binder.c src/node.c

include $(BUILD_EXECUTABLE)
