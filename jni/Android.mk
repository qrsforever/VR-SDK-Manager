LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := LeVRManager

LOCAL_ARM_MODE  := arm

LOCAL_SRC_FILES := \
	LeVRJNI.cpp \
	LeVRManager.cpp \
	LocalSocket.cpp \
	LvrSDK.cpp \
	SvrSDK.cpp \
	VrSDK.cpp \
	MobilePhoneInfo.cpp \

LOCAL_LDLIBS :=-llog
LOCAL_CFLAGS :=-DVR_DEBUG=1
# LOCAL_CFLAGS +=-DVR_BUILDTIME=\"$(shell date +%Y%m%d%H%M%S)\"

include $(BUILD_SHARED_LIBRARY)
