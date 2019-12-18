/*
===============================================================================
 Name        : SmashPad.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
//#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "lcd.h"
#include "statemachine.h"

//#include "string.h"
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
#define GPIO_JOYSTICK_PORT 	2
#define GPIO_BUTTON_PIN		22 //correct
#define GPIO_DOWN_PIN		23 //correct
#define GPIO_LEFT_PIN		25 //correct
#define GPIO_UP_PIN			26 //correct
#define GPIO_RIGHT_PIN 		27 //correct
#define GPIO_IRQ_HANDLER  			GPIO_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    GPIO_IRQn	/* GPIO interrupt NVIC interrupt name */

#define BUTTON_MASK	1<<GPIO_BUTTON_PIN
#define DOWN_MASK	1<<GPIO_DOWN_PIN
#define LEFT_MASK	1<<GPIO_LEFT_PIN
#define UP_MASK	1<<GPIO_UP_PIN
#define RIGHT_MASK	1<<GPIO_RIGHT_PIN

#define GPIO_ANALOG_PORT	0
#define GPIO_ANALOG_PORT1	1
#define GPIO_ANALOG_PIN0	23
#define GPIO_ANALOG_PIN1	24
#define GPIO_ANALOG_PIN2	25
#define GPIO_ANALOG_PIN3	26
#define GPIO_ANALOG_PIN4	30
#define GPIO_ANALOG_PIN5	31
#define GPIO_ANALOG_PIN6	12
#define GPIO_ANALOG_PIN7	13
#define LPC_ADC_IRQ ADC_IRQn
#define LPC_ADC_ID LPC_ADC
static ADC_CLOCK_SETUP_T ADCSetup;
static SSP_ConfigFormat ssp_format;

void GPIO_IRQHandler(void) {
	uint32_t value = Chip_GPIOINT_GetStatusRising(LPC_GPIOINT,GPIO_JOYSTICK_PORT);
	uint32_t value2 = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT,GPIO_JOYSTICK_PORT);
	if(value!=value2){
		switch(value2){
		case(BUTTON_MASK):{
			pressButton();
			break;
		}
		case(UP_MASK):{
			pressUp();
			break;
		}
		case(DOWN_MASK):{
			pressDown();
			break;
		}
		case(LEFT_MASK):{
			pressLeft();
			break;

		}
		case(RIGHT_MASK):{
			pressRight();
			break;

		}
		default:{
			printf("ERROR\n");
			break;
		}
		}
	}
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_JOYSTICK_PORT, 1 << GPIO_UP_PIN);
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_JOYSTICK_PORT, 1 << GPIO_DOWN_PIN);
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_JOYSTICK_PORT, 1 << GPIO_LEFT_PIN);
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_JOYSTICK_PORT, 1 << GPIO_RIGHT_PIN);
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_JOYSTICK_PORT, 1 << GPIO_BUTTON_PIN);
}

int data;
int channel = 0;
//void ADC_IRQHandler(void){
////	NVIC_DisableIRQ(LPC_ADC_IRQ);
////	NVIC_ClearPendingIRQ(LPC_ADC_IRQ);
//	if(channel==3){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH3, &data);
//		printf("data3: %d\n",data);
//		channel=4;
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH4, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH4, ENABLE);
////		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else if(channel==4){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH4, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH4, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH4, &data);
//		printf("data4: %d\n",data);
//		channel=5;
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH5, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH5, ENABLE);
////		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else if(channel==5){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH5, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH5, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH5, &data);
//		printf("data5: %d\n",data);
//		channel=6;
//
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH6, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH6, ENABLE);
////		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else{
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH6, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH6, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH6, &data);
//		printf("data6: %d\n",data);
//		channel=3;
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, ENABLE);
////		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//}



int main(void) {
#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here


	// Configure the GPIO pin as an input
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_CLEAR_PORT, GPIO_CLEAR_PIN);


	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_JOYSTICK_PORT, GPIO_DOWN_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_JOYSTICK_PORT, GPIO_LEFT_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_JOYSTICK_PORT, GPIO_RIGHT_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_JOYSTICK_PORT, GPIO_UP_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_JOYSTICK_PORT, GPIO_BUTTON_PIN);
	Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, GPIO_JOYSTICK_PORT, (1 << GPIO_UP_PIN) | (1 << GPIO_DOWN_PIN) | (1 << GPIO_LEFT_PIN) | (1 << GPIO_RIGHT_PIN) | (1 << GPIO_BUTTON_PIN));
	Chip_GPIOINT_SetIntRising(LPC_GPIOINT, GPIO_JOYSTICK_PORT, (1 << GPIO_UP_PIN) | (1 << GPIO_DOWN_PIN) | (1 << GPIO_LEFT_PIN) | (1 << GPIO_RIGHT_PIN) | (1 << GPIO_BUTTON_PIN));
//
//	Chip_ADC_Init(LPC_ADC_ID, &ADCSetup);
//	Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, ENABLE); //works
//	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN3, IOCON_ADMODE_EN, IOCON_FUNC1);
//	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT1, GPIO_ANALOG_PIN4, IOCON_ADMODE_EN, IOCON_FUNC3);
//	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT1, GPIO_ANALOG_PIN5, IOCON_ADMODE_EN, IOCON_FUNC3);
//	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN6, IOCON_ADMODE_EN, IOCON_FUNC3);
//	channel = 3;
//	Chip_ADC_SetBurstCmd(LPC_ADC_ID, ENABLE);
//	Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, ENABLE);
//	NVIC_ClearPendingIRQ(LPC_ADC_IRQ);
//	NVIC_EnableIRQ(LPC_ADC_IRQ);


	// Initialize the LCD Controller
	Board_InitLCDController();

	// Initialize the SSP
	Board_SSP_Init(LPC_SSP2);
	Chip_SSP_Init(LPC_SSP2);

	// Format SSP
	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_BITS_8;
	ssp_format.clockMode = SSP_CLOCK_MODE0;
	Chip_SSP_SetFormat(LPC_SSP2, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
	Chip_SSP_Enable(LPC_SSP2);

	// Set the DC pin
	Chip_IOCON_PinMux(LPC_GPIO, LCD_DC_PORT, LCD_DC_PIN, IOCON_FUNC0, IOCON_MODE_INACT);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DC_PORT, LCD_DC_PIN);
	NVIC_ClearPendingIRQ(GPIO_IRQn);
//	NVIC_ClearPendingIRQ(SSP2_IRQn);

	NVIC_EnableIRQ(GPIO_IRQn);




    initLCD();
	clrScr();
	setColor(0xFF, 0xFF, 0xFF);
//	drawBackground();
//	drawPadText();
//	drawPadTL(true);
//	drawPadTR(true);
//	drawPadBL(true);
//	drawPadBR(true);

   SM_init();
   SM_run();


    while(1){
    	__WFI();
    }



    return 0 ;
}
