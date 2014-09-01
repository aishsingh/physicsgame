LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libopengl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../glm \
				    $(LOCAL_PATH)/../libzip \
					$(LOCAL_PATH)/../libpng
					# $(LOCAL_PATH)/../soil/include/SOIL

LOCAL_STATIC_LIBRARIES := libzip libpng soil

LOCAL_SRC_FILES := app_glue.cpp \
					   game.cpp \
				   renderer.cpp \
				    physics.cpp \
					 object.cpp \
					 shapes.cpp \
					players.cpp \
					  trail.cpp \
					 motion.cpp \
						log.cpp

LOCAL_LDLIBS    := -llog -lGLESv2 -ldl -lz
LOCAL_CFLAGS    := -fexceptions

include $(BUILD_SHARED_LIBRARY)
# $(call import-module,soil)
