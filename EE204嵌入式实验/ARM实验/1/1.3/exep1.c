#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "sysctl.h"

#define FASTFLASHTIME (uint32_t)500000
#define SLOWFLASHTIME (uint32_t)4000000

uint32_t delay_time, key_value1, key_value2;

void Delay(uint32_t value);
void S800_GPIO_Init(void);

int main(void)
{
	S800_GPIO_Init();
	while (1)
	{
		if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x1);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
		}

		if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) == 0)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x2);//have to set the second bit, better to use the const "GPIO_PIN_X" for "GPIO_PIN_X"
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
		}
	}
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for (ui32Loop = 0; ui32Loop < value; ui32Loop++)
	{
	};
}

void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable PortF
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
		;																					 //Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); //Enable PortJ
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
		; //Wait for the GPIO moduleJ ready

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Set PF0 as Output pin

	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
