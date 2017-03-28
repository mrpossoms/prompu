SRC+= main.c i2c_util.c drv_LSM9DS0.c MARG.c

include boards/TM4C123G/vars.mk
#include boards/propeller.mk

#SERIAL=cu.usbserial-*
SERIAL=/dev/ttyUSB0
CFLAGS+=-std=c11 -I./src -g -O0 -D DEBUG
OUTDIR=./out
TARGET=prompu


# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SRC:.c=.o)))

#.PHONY:
#what:
#	echo $(SRC)
#	echo $(OBJECTS)

# default: build bin
all: $(OUTDIR)/$(TARGET).bin

$(OUTDIR)/%.o: src/boards/$(MCU)/%.c | $(OUTDIR)
	$(CC) -o $@ -c $^ $(INC) $(CFLAGS)

$(OUTDIR)/%.o: src/%.c | $(OUTDIR)
	$(CC) -o $@ -c $^ $(INC) $(CFLAGS)
	#$(CC) $(CFLAGS) -c $^ 

$(OUTDIR)/a.out: $(OBJECTS)
	$(CC) $(CFLAGS)  $(LDFLAGS) $^ -o $@ $(LINK) 

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/a.out
	$(OBJCOPY) -O binary $< $@

$(OUTDIR):
	mkdir $(OUTDIR)

.PHONY: flash

check: all
	propeller-elf-objdump -h out/firmware.elf

flash: all
	sudo lm4flash out/prompu.bin
	#propeller-load -p $(SERIAL) -S20 -I $(PROP_GCC)/propeller-load out/firmware.elf -r

run: flash
	screen $(SERIAL) 115200

clean:
	rm -rf $(OUTDIR)
