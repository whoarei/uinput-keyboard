LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := uinput
LOCAL_SRC_FILES := main.cpp Command.cpp UInput.cpp tinyxml2.cpp Parser.cpp
	
LOCAL_SHARED_LIBRARIES := libutils libstlport
#LOCAL_STATIC_LIBRARIES := libtinyxml2

include external/stlport/libstlport.mk

include $(BUILD_EXECUTABLE)
