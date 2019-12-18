#include "qpn_port.h"
#include "board.h"

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
#define UART_SELECTION 	LPC_UART0
#define IRQ_SELECTION 	UART0_IRQn
#define HANDLER_NAME 	UART0_IRQHandler
/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 32	/* Receive */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

static ADC_CLOCK_SETUP_T ADCSetup;

enum UI_FLAG_TAG {
	NOTHING,
	DRAW,
	ERASE
};
typedef enum UI_FLAG_TAG UI_FLAG;

#ifndef SRC_STATE_MACHINE_H_
#define SRC_STATE_MACHINE_H_
// declare struct for state machine
typedef struct SM_Tag {
	QActive super;

	int padSelect;
	int hover;
	int smashColor;
	// draw/erase flags
	UI_FLAG gBackground;
	UI_FLAG gSmashPadText;
	UI_FLAG gPadText;
	UI_FLAG gPadTL;
	UI_FLAG gPadTL_unpressed;
	UI_FLAG gPadTL_pressed;
	UI_FLAG gPadTR;
	UI_FLAG gPadTR_unpressed;
	UI_FLAG gPadTR_pressed;
	UI_FLAG gPadBL;
	UI_FLAG gPadBL_unpressed;
	UI_FLAG gPadBL_pressed;
	UI_FLAG gPadBR;
	UI_FLAG gPadBR_unpressed;
	UI_FLAG gPadBR_pressed;
	UI_FLAG gPadHighlight;
	UI_FLAG gSoundSelectText;
	UI_FLAG gSounds;



} SM;

extern SM Main_SM;

void SM_init();
void SM_run();
void pressButton();
void pressDown();
void pressUp();
void pressLeft();
void pressRight();
void timer0();
void timer1();
void timer2();
void timer3();
void SMASHRED();
void SMASHGREEN();
void SMASHYELLOW();
void SMASHBLUE();
void SMASH(int r,int g, int y, int b);

#endif /* SRC_STATE_MACHINE_H_ */
