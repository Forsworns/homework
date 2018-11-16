#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "sysctl.h"

int main(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(1);
}