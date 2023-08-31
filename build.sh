#!/bin/sh

CC="gcc -pipe"
CFLAGS="-Wall -Wextra -Wpedantic -s -Os -fsanitize=address"
# largely based on https://github.com/Francesco149/nolibc-httpd
CFLAGS_NOLIBC="-DNOLIBC -Wall -Wextra -Wpedantic -s -Os -flto \
    -nostdlib -ffreestanding -no-pie -fno-stack-protector \
    -fdata-sections -ffunction-sections -fno-unwind-tables -fno-asynchronous-unwind-tables \
	-Wl,-n -Wl,--gc-sections -Wl,--build-id=none"

[ "$NOLIBC" ] && $CC $CFLAGS_NOLIBC coin.c crt.S -o coin || $CC $CFLAGS coin.c -o coin
strip -R .comment ./coin
