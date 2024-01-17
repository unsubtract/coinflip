#!/bin/sh

CC="gcc"
CFLAGS="-pipe -Wall -Wextra -Wpedantic -std=c99 -s -Os -flto"
# largely based on https://github.com/Francesco149/nolibc-httpd
CFLAGS_NOLIBC="-DNOLIBC -nostdlib -ffreestanding -no-pie --entry=main
    -fno-stack-protector -fdata-sections -ffunction-sections -fno-unwind-tables
    -fno-asynchronous-unwind-tables -Wl,-n -Wl,--gc-sections -Wl,--build-id=none"

$CC $CFLAGS ${NOLIBC:+$CFLAGS_NOLIBC crt.S} coin.c -o coin
strip -R .comment coin
