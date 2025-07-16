#/bin/bash
/usr/bin/arm-linux-gnueabi-gcc \
 -I . -I ./lvgl -I ./lvgl/src -I ./lvgl/src/core -I ./eez/src/ui \
 ./lvgl/src/*.c ./lvgl/src/core/*.c \
 *.c eez/src/ui/*.c -lrt -lm \
 -o build/x32ui
