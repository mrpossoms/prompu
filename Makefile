CC=propeller-elf-gcc
C_FLAGS= -m32bit-doubles -Os -mcmm -std=c99 -s
INC=-Ilib/libsimpletools -Ilib/libsimpletext
LIB=-Llib/libsimpletools/cmm -Llib/libsimpletext/cmm
LINK=-lm -lsimpletext
SRC=src/i2c_util.c src/drv_LSM9DS0.c src/main.c
#SERIAL=cu.usbserial-*
SERIAL=/dev/ttyUSB0

#
all: i2c_util drv_LSM9DS0 basic.naive 
	$(CC) $(INC) $(LIB) -o out/firmware.elf $(C_FLAGS) out/*.o src/main.c $(LINK)
#

drv_LSM9DS0:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o

i2c_util:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o

MARG:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o

basic.naive:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o

.PHONY: flash

check: all
	propeller-elf-objdump -h out/firmware.elf

flash: check
	propeller-load -p $(SERIAL) -S20 -I $(PROP_GCC)/propeller-load out/firmware.elf -r

run: flash
	screen $(SERIAL) 115200

clean:
	rm out/*.o out/*.elf out/*.binary
