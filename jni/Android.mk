LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libopengl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/glm

LOCAL_SRC_FILES := app_glue.cpp \
					   game.cpp \
				   renderer.cpp \
				    physics.cpp \
					 object.cpp \
					 shapes.cpp \
					players.cpp \
					 motion.cpp \
						log.cpp

LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS    := -fexceptions

include $(BUILD_SHARED_LIBRARY)
