LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libopengl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../glm \
				    $(LOCAL_PATH)/../libzip \
					$(LOCAL_PATH)/../libpng

LOCAL_STATIC_LIBRARIES := libzip libpng googletest_main

LOCAL_SRC_FILES := \
					app_glue.cpp \
					game.cpp \
					renderer.cpp \
					obj_renderer.cpp \
					asset_renderer.cpp \
					screen_renderer.cpp \
					player_renderer.cpp \
					background_renderer.cpp \
					math.cpp \
					physics.cpp \
					texture_handler.cpp \
					input_handler.cpp \
					collision.cpp \
					joystick.cpp \
					rect.cpp \
					object.cpp \
					grav_object.cpp \
					shape.cpp \
					box.cpp \
					player.cpp \
					spaceman.cpp \
					trail.cpp \
					planet.cpp \
					star.cpp \
					galaxy.cpp \
					motion.cpp \
					camera.cpp \
					colour.cpp \
					log.cpp \
					./unittests/test_physics.cpp \
					./unittests/tests.cpp

LOCAL_LDLIBS := -llog -lGLESv2 -ldl -lz
LOCAL_CFLAGS := -fexceptions -std=c++98

include $(BUILD_SHARED_LIBRARY)
