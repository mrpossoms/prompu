CC=propeller-elf-gcc
CFLAGS=-DTARGET_IS_PROPELLER -m32bit-doubles -Os -mcmm -std=c99 -s
INC=-Ilib/libsimpletools -Ilib/libsimplei2c -Ilib/libsimpletext
LIB=-Llib/libsimpletools/cmm -Llib/libsimpletext/cmm
LINK=-lm -lsimpletext
