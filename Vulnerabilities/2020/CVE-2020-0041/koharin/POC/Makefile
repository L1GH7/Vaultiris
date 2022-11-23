NDK_BUILD := NDK_PROJECT_PATH=. ndk-build NDK_APPLICATION_MK=./Application.mk

all: android 

android:
	$(NDK_BUILD)
push:
	adb push ./libs/arm64-v8a/poc /data/local/tmp/poc
clean:
	$(NDK_BUILD) clean
	rm -rf obj libs
	adb shell rm /data/local/tmp/poc

