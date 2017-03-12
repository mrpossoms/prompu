CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

MCU = TM4C123GH6PM

# INCLUDES: list of includes, by default, use Includes directory
INC = -Isrc/boards/$(MCU)/

# TIVAWARE_PATH: path to tivaware folder
TIVAWARE_PATH = /opt/tivaware

# LD_SCRIPT: linker script
LD_SCRIPT = boards/TM4C123G/$(MCU).ld

LINK += -lm -ldriver
SRC += boards/$(MCU)/uart.c boards/$(MCU)/startup_gcc.c boards/$(MCU)/i2c_util.c boards/$(MCU)/uartstdio.c
# define flags
CFLAGS = -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall
CFLAGS += -pedantic -DPART_$(MCU) -I$(TIVAWARE_PATH)
CFLAGS += -DTARGET_IS_TI_LAUNCHPAD -DTARGET_IS_BLIZZARD_RA1 

LDFLAGS = -L/opt/tivaware/driverlib/gcc -Wl,--script=$(LD_SCRIPT) -Wl,--entry=ResetISR -Wl,--gc-sections 

