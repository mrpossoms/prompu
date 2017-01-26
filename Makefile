CC=propeller-elf-gcc
C_FLAGS= -m32bit-doubles -Os -mcmm -std=c99 
INC=-I./src -Ilib/libsimplei2c -Ilib/libsimpletools -Ilib/libsimpletext
LIB=-L./src -Llib/libsimplei2c/cmm -Llib/libsimpletools/cmm -Llib/libsimpletext/cmm
LINK=-lm -lsimplei2c -lsimpletools -lsimpletext
SRC=src/i2c_util.c src/drv_LSM9DS0.c src/main.c

all: i2c_util drv_LSM9DS0
	$(CC) $(INC) $(LIB) -o out/firmware.elf $(C_FLAGS) out/*.o src/main.c $(LINK)

drv_LSM9DS0:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o

i2c_util:
	$(CC) $(INC) $(LIB) $(C_FLAGS) -c src/$@.c -o out/$@.o


.PHONY: flash

flash: all
	propeller-load -s out/firmware.elf
	propeller-elf-objdump -h out/firmware.elf
	propeller-load -I $(PROP_GCC)/propeller-load out/firmware.elf -r
