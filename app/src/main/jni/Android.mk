LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := changle
LOCAL_SRC_FILES := changle.c  GraphDSP.c

include $(BUILD_SHARED_LIBRARY)