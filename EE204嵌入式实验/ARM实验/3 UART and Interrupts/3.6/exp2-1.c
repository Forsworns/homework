
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_i2c.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "hw_ints.h"
#include "i2c.h"
#include "pin_map.h"
#include "sysctl.h"
#include "systick.h"
#include "interrupt.h"
#include "uart.h"
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

#define MAX_STRING 80


void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
uint8_t 	I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t 	I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);
void		S800_I2C0_Init(void);
void 		S800_SYSTICK_Init(void);
void 		S800_UART_Init(void);
void 		GPIOJ_Handler(void);
void 		SysTick_Handler(void);
void    UARTStringPut(uint8_t *);
void 		UARTStringPutNonBlocking(const char *);
void 		UART0_Handler(void);
volatile uint8_t result;
uint32_t ui32SysClock;

#define digitColon1 0x4
#define digitColon2 0x20
#define segColon 0x09
#define digitHH 0x1
#define digitH 0x2
#define digitMM 0x8
#define digitM 0x10
#define digitSS 0x40
#define digitS 0x80
uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};
uint8_t segHH=0;
uint16_t segH=0;
uint32_t segMM=0;
uint32_t segM=0;
uint32_t segSS=0;
uint32_t segS=0;
uint8_t prev1 = 1;
uint8_t now1 = 1;
uint8_t prev2 = 1;
uint8_t now2 = 1;
uint8_t flag = 0;
#define PRESSED 0

#define  KEY_PERIPH             SYSCTL_PERIPH_GPIOJ
#define  KEY_PORT               GPIO_PORTJ_BASE
#define  KEY_PIN                GPIO_PIN_1

char re_char;
char re_string[80]={'\0'};
const char* GETTIME="GETTIME";
const char* SET="SET";
const char* INC="INC";
int main(void)
{
	//jtagWait();
  ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 16000000);		
	S800_GPIO_Init();
	S800_I2C0_Init();
	S800_SYSTICK_Init();
	S800_UART_Init();
	while(1){
		now1 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0);
		now2 = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1);
		
		if(prev1!=now1 && now1==PRESSED){
			segS++;
		}
		prev1 = now1;
		
		if(prev2!=now2 && now2==PRESSED){
			segS+=60;
		}
		prev2 = now2;
		
		if(now1 != PRESSED && now2 != PRESSED){
			SysTickDisable();
			SysTickPeriodSet(ui32SysClock);
			while(SysTickPeriodGet()!=ui32SysClock);
			flag = 0;
			SysTickEnable();
		}else if(now1 == PRESSED && now2 != PRESSED){
			SysTickDisable();
			SysTickPeriodSet(ui32SysClock/5);
			while(SysTickPeriodGet()!=ui32SysClock/5);
			flag = 1;
			SysTickEnable();
		}else if(now1 != PRESSED && now2 == PRESSED){
			SysTickDisable();
			SysTickPeriodSet(ui32SysClock/5);
			while(SysTickPeriodGet()!=ui32SysClock/5);
			flag = 2;
			SysTickEnable();
		}else{
			SysTickDisable();
			SysTickPeriodSet(ui32SysClock/5);
			while(SysTickPeriodGet()!=ui32SysClock/5);
			flag = 3;
			SysTickEnable();
		}
		
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitHH);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segHH%6]);		// Display number
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitH);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segH%10]);		// Display number
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,digitColon1);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,segColon);
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitMM);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segMM%6]);		// Display number
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitM);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segM%10]);		// Display number
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,digitColon2);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,segColon);
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitSS);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segSS%6]);		// Display number
		Delay(10000);
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, digitS);			// Select digit
		I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[segS%10]);		// Display number
		Delay(10000);
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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);						//Enable PortN
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));			//Wait for the GPIO moduleN ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);							//Set PF0 as Output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);							//Set PN0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);	//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void S800_SYSTICK_Init(void)
{
	SysTickPeriodSet(ui32SysClock);
	SysTickEnable();
	SysTickIntEnable();
	//SysTickIntRegister(SysTick_Handler);//register ISR必须在enable INT之后，成了，jtag锁是因为这个函数
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

void S800_UART_Init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE,ui32SysClock,115200,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
	UARTStringPut((uint8_t *)"\r\nHello, world!\r\n");

	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	IntPrioritySet(INT_UART0,0x0);
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

void SysTick_Handler(void)
{
	if(flag==2)segS+=60;
	else if(flag==3)segS+=61;
	else segS++;
	
	segSS = segS/10;
	segM = segSS/6;
	segMM = segM/10;
	segH = segMM/6;
	segHH = segH/10;
}

void UARTStringPut(uint8_t *cMessage){
	while(*cMessage!='\0')
		UARTCharPut(UART0_BASE,*(cMessage++));
}

void UARTStringPutNonBlocking(const char *cMessage){
	while(*cMessage!='\0')
		UARTCharPutNonBlocking(UART0_BASE,*(cMessage++));
}

void UART0_Handler(void){
	int32_t uart0_int_status;
	int8_t length=0;
	uart0_int_status = UARTIntStatus(UART0_BASE,true);
	UARTIntClear(UART0_BASE,uart0_int_status);
	while(UARTCharsAvail(UART0_BASE)){
		re_char = UARTCharGetNonBlocking(UART0_BASE);
		re_string[length] = re_char;
		re_string[length+1] = '\0';
		length++;
		Delay(30*length);
	}
	if(strcmp(re_string,GETTIME)==0){
		re_string[0]='T';
		re_string[1]='I';
		re_string[2]='M';
		re_string[3]='E';
		re_string[4] = segHH%6+'0';
		re_string[5] = segH%10+'0';
		re_string[6] = ':';
		re_string[7] = segMM%6+'0';
		re_string[8] = segM%10+'0';
		re_string[9] = ':';
		re_string[10] = segSS%6+'0';
		re_string[11] = segS%10+'0';
		re_string[12] = '\0';
		UARTStringPutNonBlocking(re_string);
	}else if(strstr(re_string,SET)==re_string){
		int8_t i;
		int32_t sum = 0;
		sum+=(re_string[3]-'0')*10*3600;
		sum+=(re_string[4]-'0')*3600;
		sum+=(re_string[6]-'0')*10*60;
		sum+=(re_string[7]-'0')*60;
		sum+=(re_string[9]-'0')*10;
		sum+=(re_string[10]-'0');
		segS = (sum-1);//减一是为了矫正时钟ISR的自增操作
		re_string[12] = '\0';
		for(i=11;i>=4;--i){
			re_string[i] = re_string[i-1];
		}
		re_string[0]='T';
		re_string[1]='I';
		re_string[2]='M';
		re_string[3]='E';
		UARTStringPutNonBlocking(re_string);
	}else if(strstr(re_string,INC)==re_string){
		int32_t sum = 0;
		sum+=(re_string[3]-'0')*10*3600;
		sum+=(re_string[4]-'0')*3600;
		sum+=(re_string[6]-'0')*10*60;
		sum+=(re_string[7]-'0')*60;
		sum+=(re_string[9]-'0')*10;
		sum+=(re_string[10]-'0');
		segS += (sum-1);
		segSS = segS/10;
		segM = segSS/6;
		segMM = segM/10;
		segH = segMM/6;
		segHH = segH/10;
		re_string[0]='T';
		re_string[1]='I';
		re_string[2]='M';
		re_string[3]='E';
		re_string[4] = segHH%6+'0';
		re_string[5] = segH%10+'0';
		re_string[6] = ':';
		re_string[7] = segMM%6+'0';
		re_string[8] = segM%10+'0';
		re_string[9] = ':';
		re_string[10] = segSS%6+'0';
		re_string[11] = segS%10+'0';
		re_string[12] = '\0';
		UARTStringPutNonBlocking(re_string);
	}else{
		UARTStringPutNonBlocking(re_string);
	}
}
