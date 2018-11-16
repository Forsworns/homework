
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_i2c.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "i2c.h"
#include "pin_map.h"
#include "sysctl.h"
//*****************************************************************************
//
//I2C GPIO chip address and resigster define
//
//*****************************************************************************
#define TCA6424_I2CADDR 					0x22
#define PCA9557_I2CADDR						0x18

#define PCA9557_INPUT							0x00
#define	PCA9557_OUTPUT						0x01
#define PCA9557_POLINVERT					0x02
#define PCA9557_CONFIG						0x03

#define TCA6424_CONFIG_PORT0			0x0c
#define TCA6424_CONFIG_PORT1			0x0d
#define TCA6424_CONFIG_PORT2			0x0e

#define TCA6424_INPUT_PORT0				0x00
#define TCA6424_INPUT_PORT1				0x01
#define TCA6424_INPUT_PORT2				0x02

#define TCA6424_OUTPUT_PORT0			0x04
#define TCA6424_OUTPUT_PORT1			0x05
#define TCA6424_OUTPUT_PORT2			0x06




void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
uint8_t 	I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t 	I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);
void		S800_I2C0_Init(void);
volatile uint8_t result;
uint32_t ui32SysClock;
int i;
uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};
uint8_t DIGIT[] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};
                                             
int main(void)
{
	uint32_t times = 0;
	//use internal 16M oscillator, HSI
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 16000000);		
	
		S800_GPIO_Init();
		S800_I2C0_Init();
	
		while (1)	{
			if(!GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)){continue;}
			for(i = 200; i > 0; --i)
			{
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, DIGIT[times%8]);			// Select digits
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[times%8]);				// Display number 		
				I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~DIGIT[times%8]);
				Delay(20000);
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, DIGIT[(times+1)%8]);			// Select digits
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[(times+1)%8]);				// Display number 		
				I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~DIGIT[(times+1)%8]);
				Delay(20000);
			}
			times++;
		}
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);							//Set PF0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);	//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}


void S800_I2C0_Init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);			// Provide clock to enable I2C0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		// I2C0 uses pin PB2 and PB3, which by default are used as GPIO. We need to program to change the use of PB2 and PB3
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);							// Program on GPIOPCTL to select I2C0 SCL to be connected to PB2
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);							// Program on GPIOPCTL to select I2C0 SDA to be connected to PB3
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);	// Program on GPIOAFSEL to swith PB2 to be used by hardware and program the pad control of this pin for this
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);		// Program on GPIOAFSEL to swith PB3 to be used by hardware and program the pad control of this pin for this

	I2CMasterInitExpClk(I2C0_BASE,ui32SysClock, true);						//enable I2C0 master, the input clock for I2C0 is the system clock and set I2C0 to run in fast mode (400kbps) or normal mode

	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT0,0x0ff);		//config port0 of TCA6424 as input
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT1,0x0);			//config port1 of TCA6424 as output
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT2,0x0);			//config port2 of TCA6424 as output

	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0xff);		//Select all digits
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x0);			//Turn off all LED tubes
	


	I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_CONFIG,0x0);						//config the port of PCA9557 as output
	I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);					//turn off the LED1-8
	
}


uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData)
{
	uint8_t rop;
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy
		
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); // Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)		
	I2CMasterDataPut(I2C0_BASE, RegAddr);							// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);	// Start the transmission and send multiple bytes
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy
		
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);						// if there is error in the transmission, terminate the transmission
	if(rop) {
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);
		return rop;
	}

	I2CMasterDataPut(I2C0_BASE, WriteData);						// Set the data (the data to the register specified by the above command) to be sent
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);// send the second byte and terminate the transmission when finished
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy

	rop = (uint8_t)I2CMasterErr(I2C0_BASE);						// if there is error in the transmission, return the error status
	return rop;
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
	uint8_t value,rop;
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy

	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterDataPut(I2C0_BASE, RegAddr);							// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND);	// Start the transmission and send one byte
	while(I2CMasterBusBusy(I2C0_BASE));								// Wait if I2C0 master is busy
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	if(rop) {																					// if there is error in the transmission, return the error status
		return rop;
	}
	Delay(1);

	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);// Start the transmission and read one byte
	while(I2CMasterBusBusy(I2C0_BASE));								// Wait if I2C0 master is busy
	value=I2CMasterDataGet(I2C0_BASE);								// Get the data from the data register of the master
	Delay(1);
	return value;
}

