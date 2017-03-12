#ifndef __BOARD_SUPPORT

#ifdef TARGET_IS_PROPELLER
#include <propeller.h>
#include "simplei2c.h"

#ifdef DEBUG
#include "simpletext.h"
#endif

#endif

#ifdef TARGET_IS_TI_LAUNCHPAD
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/fpu.h"
#include "uartstdio.h"

#endif

#endif
