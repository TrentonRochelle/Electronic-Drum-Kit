#include "statemachine.h"
#include "qpn_port.h"
#include "string.h";
#define TICKRATE_HZ1 (20)	/* 10 ticks per second */
#define CHSEL (1UL<<0 | 1UL<<1 | 1UL<<2 | 1UL<<3)

enum Signals{
	UP=Q_USER_SIG,
	DOWN,
	LEFT,
	RIGHT,
	BUTTON,
	T_TL,
	T_TR,
	T_BL,
	T_BR,
	T_COLOR,
	TL,
	TR,
	BL,
	BR
};

char uart_red[] = "a";
char uart_green[] ="d";
char uart_yellow[] = "g";
char uart_blue[] = "l";
int red_signal_i=0;
int green_signal_i=3;
int yellow_signal_i=6;
int blue_signal_i=11;

const char *uart_signal[]={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t"};


static QEvent l_lab2aQueue[1000];

SM Main_SM;

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
	{ (QActive *)0,            (QEvent *)0,          0                    },
	{ (QActive *)&Main_SM,    l_lab2aQueue,         Q_DIM(l_lab2aQueue)  }
};

void HANDLER_NAME(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(UART_SELECTION, &rxring, &txring);
}

void TIMER0_IRQHandler(void)
{
//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
//		Chip_TIMER_Reset(LPC_TIMER0);
//		printf("timer0\n");
//		timer0();
//	}
	Chip_TIMER_Disable(LPC_TIMER0);
	Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
	Chip_TIMER_Reset(LPC_TIMER0);
//	printf("timer0\n");
	timer0();

}
void TIMER1_IRQHandler(void)
{
	Chip_TIMER_Disable(LPC_TIMER1);
	Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
	Chip_TIMER_Reset(LPC_TIMER1);
//	printf("timer1\n");
	timer1();
}
void TIMER2_IRQHandler(void)
{
	Chip_TIMER_Disable(LPC_TIMER2);
	Chip_TIMER_ClearMatch(LPC_TIMER2, 1);
	Chip_TIMER_Reset(LPC_TIMER2);
//	printf("timer2\n");
	timer2();
}
void TIMER3_IRQHandler(void)
{
	Chip_TIMER_Disable(LPC_TIMER3);
	Chip_TIMER_ClearMatch(LPC_TIMER3, 1);
	Chip_TIMER_Reset(LPC_TIMER3);
//	printf("timer3\n");
	timer3();
}
//int data;
//int channel = 0;
//void ADC_IRQHandler(void){
//	NVIC_DisableIRQ(LPC_ADC_IRQ);
//	NVIC_ClearPendingIRQ(LPC_ADC_IRQ);
//	if(channel==3){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH3, &data);
//		printf("data3: %d\n",data);
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH4, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH4, ENABLE);
//		channel=4;
//		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else if(channel==4){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH4, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH4, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH4, &data);
//		printf("data4: %d\n",data);
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH5, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH5, ENABLE);
//		channel=5;
//		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else if(channel==5){
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH5, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH5, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH5, &data);
//		printf("data5: %d\n",data);
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH6, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH6, ENABLE);
//		channel=6;
//		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//	else{
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH6, DISABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH6, DISABLE);
//		Chip_ADC_ReadValue(LPC_ADC, ADC_CH6, &data);
//		printf("data6: %d\n",data);
//		Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, ENABLE);
//		Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, ENABLE);
//		channel=3;
//		NVIC_EnableIRQ(LPC_ADC_IRQ);
//		return;
//	}
//}





void SM_ctor();
QState SM_state_init(SM* me);
QState SM_state_main(SM* me);
QState SM_state_padSelect(SM* me);
QState SM_state_soundSelect(SM* me);


void SM_init() {
	SM_ctor();
}

void SM_run() {
	QF_run();
}

void pressButton(){
	QActive_post((QActive*)(&Main_SM), BUTTON);

}

void pressDown(){
	QActive_post((QActive*)(&Main_SM), DOWN);

}

void pressUp(){
	QActive_post((QActive*)(&Main_SM), UP);

}
void pressLeft(){
	QActive_post((QActive*)(&Main_SM), LEFT);

}

void pressRight(){
	QActive_post((QActive*)(&Main_SM), RIGHT);

}

void timer0(){
	QActive_post((QActive*)(&Main_SM), T_TL);
}
void timer1(){
	QActive_post((QActive*)(&Main_SM), T_TR);
}
void timer2(){
	QActive_post((QActive*)(&Main_SM), T_BL);
}
void timer3(){
	QActive_post((QActive*)(&Main_SM), T_BR);
}


void SMASHRED(){
	Chip_UART_SendRB(UART_SELECTION, &txring, uart_red, sizeof(uart_red)-1);
	QActive_post((QActive*)(&Main_SM), TL);
//	printf("SMASHRED\n");
}
void SMASHGREEN(){
	Chip_UART_SendRB(UART_SELECTION, &txring, uart_green, sizeof(uart_green)-1);
	QActive_post((QActive*)(&Main_SM), TR);
//	printf("SMASHGREEN\n");
}
void SMASHYELLOW(){
	Chip_UART_SendRB(UART_SELECTION, &txring, uart_yellow, sizeof(uart_yellow)-1);
	QActive_post((QActive*)(&Main_SM), BL);
//	printf("SMASHYELLOW\n");
}
void SMASHBLUE(){
	Chip_UART_SendRB(UART_SELECTION, &txring, uart_blue, sizeof(uart_blue)-1);
	QActive_post((QActive*)(&Main_SM), BR);
//	printf("SMASHBLUE\n");
}
void SMASH(int r,int g, int y, int b){
		if(y>0){
			SMASHYELLOW();
		}
		if(g>0){
			SMASHGREEN();
		}
		if(b>0){
			SMASHBLUE();
		}
		if(r>0){
			SMASHRED();
		}



}


void SM_ctor() {
	SM* me = &Main_SM;
//	printf("Constructing SM\n");
	me->padSelect = 0;
	me->smashColor = 0;
	me->gBackground=NOTHING;
	me->gSmashPadText = NOTHING;
	me->gPadText=NOTHING;
	me->gPadTL = NOTHING;
	me->gPadTL_unpressed = NOTHING;
	me->gPadTL_pressed = NOTHING;
	me->gPadTR = NOTHING;
	me->gPadTR_unpressed = NOTHING;
	me->gPadTR_pressed = NOTHING;
	me->gPadBL = NOTHING;
	me->gPadBL_unpressed = NOTHING;
	me->gPadBL_pressed = NOTHING;
	me->gPadBR = NOTHING;
	me->gPadBR_unpressed = NOTHING;
	me->gPadBR_pressed = NOTHING;
	me->gPadHighlight = NOTHING;
	me->gSoundSelectText = NOTHING;

	Chip_TIMER_Init(LPC_TIMER0);
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_Init(LPC_TIMER2);
	Chip_TIMER_Init(LPC_TIMER3);
	uint32_t timerFreq = Chip_Clock_GetSystemClockRate();
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_Reset(LPC_TIMER2);
	Chip_TIMER_Reset(LPC_TIMER3);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER2, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER3, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_SetMatch(LPC_TIMER2, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_SetMatch(LPC_TIMER3, 1, (timerFreq / TICKRATE_HZ1));
	Chip_ADC_Init(LPC_ADC_ID, &ADCSetup);
//	Chip_ADC_EnableChannel(LPC_ADC_ID, ADC_CH3, ENABLE); //works
//	Chip_ADC_Int_SetChannelCmd(LPC_ADC_ID, ADC_CH3, ENABLE);
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN0, IOCON_ADMODE_EN, IOCON_FUNC1); //Yellow WORKS
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN1, IOCON_ADMODE_EN, IOCON_FUNC1); //Green WORKS
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN2, IOCON_ADMODE_EN, IOCON_FUNC1); //Blue WORKS?
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_ANALOG_PORT, GPIO_ANALOG_PIN3, IOCON_ADMODE_EN, IOCON_FUNC1); //Red WORKS
//	channel = 3;
//	Chip_ADC_SetBurstCmd(LPC_ADC_ID, ENABLE);
//	NVIC_ClearPendingIRQ(LPC_ADC_IRQ);
//	NVIC_EnableIRQ(LPC_ADC_IRQ);




	Board_UART_Init(UART_SELECTION);
	Chip_UART_SetBaud(UART_SELECTION, 115200);
	Chip_UART_ConfigData(UART_SELECTION, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(UART_SELECTION, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(UART_SELECTION);
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Reset and enable FIFOs, FIFO trigger level 3 (14 chars) */
	Chip_UART_SetupFIFOS(UART_SELECTION, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
							UART_FCR_TX_RS | UART_FCR_TRG_LEV3));

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(UART_SELECTION, (UART_IER_RBRINT | UART_IER_RLSINT));
	NVIC_SetPriority(IRQ_SELECTION, 1);
	NVIC_ClearPendingIRQ(IRQ_SELECTION);
	NVIC_EnableIRQ(IRQ_SELECTION);
//	Chip_ADC_SetStartMode(LPC_ADC_ID, ADC_START_NOW, ADC_TRIGGERMODE_RISING);


//	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);
//	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
//	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 1);
//	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER3, 1);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER2_IRQn);
	NVIC_EnableIRQ(TIMER3_IRQn);

	QActive_ctor(&me->super, (QStateHandler)&SM_state_init);

}


QState SM_state_init(SM *me) {
//	printf("Init\n");
	me->gBackground = DRAW;
	me->padSelect = 1;
    return Q_TRAN(&SM_state_main);
}

QState SM_state_on(SM* me) {
	switch(Q_SIG(me)) {
	case Q_INIT_SIG:
		return Q_TRAN(&SM_state_main);
//	case BUTTON_PRESSED:
//		if (me->buttonState == 0 && me->buttonWaited) {
//			me->buttonWaited = 0;
//			XTmrCtr_Start(&per_timer, 0);
//			XGpio_DiscreteWrite(&per_leds, 1, 0xFFFF);
//			QActive_post((QActive*)(&SM_SM), BUTTON_CLICK);
//			me->buttonState = 1;
//		}
//		return Q_HANDLED();
//	case BUTTON_RELEASED:
//		me->buttonState = 0;
//		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

QState SM_state_main(SM *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			me->gSmashPadText = DRAW;
			me->gPadTL = DRAW;
			me->gPadTL_unpressed = DRAW;
			me->gPadTR = DRAW;
			me->gPadTR_unpressed = DRAW;
			me->gPadBL = DRAW;
			me->gPadBL_unpressed = DRAW;
			me->gPadBR = DRAW;
			me->gPadBR_unpressed = DRAW;
			return Q_HANDLED();
		}

		case Q_INIT_SIG: {
			return Q_HANDLED();
		}
		case BUTTON:{
//			printf("button\n");
			return Q_TRAN(&SM_state_padSelect);
		}
		case TL:{
			me->gPadTL_pressed = DRAW;
			me->smashColor=0;
			me->gSmashPadText = DRAW;
			Chip_TIMER_Enable(LPC_TIMER0);
			return Q_HANDLED();
		}
		case BR:{
			me->smashColor=3;
			me->gSmashPadText = DRAW;
			me->gPadBR_pressed = DRAW;
			Chip_TIMER_Enable(LPC_TIMER3);
			return Q_HANDLED();
		}
		case BL:{
			me->smashColor=2;
			me->gSmashPadText = DRAW;
			me->gPadBL_pressed = DRAW;
			Chip_TIMER_Enable(LPC_TIMER2);
			return Q_HANDLED();
		}
		case TR:{
			me->smashColor=1;
			me->gSmashPadText = DRAW;
			me->gPadTR_pressed = DRAW;
			Chip_TIMER_Enable(LPC_TIMER1);
			return Q_HANDLED();
		}
		case T_TL:{

			me->gPadTL_unpressed = DRAW;
			return Q_HANDLED();
		}
		case T_TR:{
			me->gPadTR_unpressed = DRAW;
			return Q_HANDLED();
		}
		case T_BL:{
			me->gPadBL_unpressed = DRAW;
			return Q_HANDLED();
		}
		case T_BR:{
			me->gPadBR_unpressed = DRAW;
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
//			me->gSmashPadText = ERASE;
			drawBackground();
			return Q_HANDLED();
		}

	}
	return Q_SUPER(&SM_state_on);
}
QState SM_state_padSelect(SM *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
//			printf("Main-entry\n");
			me->gPadText = DRAW;
			me->gPadHighlight= DRAW;
			me->gPadTL = DRAW;
			me->gPadTR = DRAW;
			me->gPadBL = DRAW;
			me->gPadBR = DRAW;

			return Q_HANDLED();
		}

		case Q_INIT_SIG: {
			return Q_HANDLED();
		}
		case BUTTON:{
			printf("button\n");
			return Q_TRAN(&SM_state_soundSelect);
		}
		case UP:{
//			printf("UP\n");
//			me->gPadHighlight = ERASE;
			erasePadHighlight(me->padSelect);
			switch(me->padSelect){
			case(1):{ //TL
				me->padSelect=3;
				break;
			}
			case(2):{ //TR
				me->padSelect=4;
				break;
			}
			case(3):{ //BL
				me->padSelect=1;
				break;
			}
			case(4):{ //BR
				me->padSelect=2;
				break;
			}
			}
			me->gPadHighlight = DRAW;
			return Q_HANDLED();
		}
		case DOWN:{
//			printf("DOWN\n");
//			me->gPadHighlight = ERASE;
			erasePadHighlight(me->padSelect);

			switch(me->padSelect){
			case(1):{ //TL
				me->padSelect=3;
				break;
			}
			case(2):{ //TR
				me->padSelect=4;
				break;
			}
			case(3):{ //BL
				me->padSelect=1;
				break;
			}
			case(4):{ //BR
				me->padSelect=2;
				break;
			}
			}
			me->gPadHighlight = DRAW;
			return Q_HANDLED();
		}
		case LEFT:{
//			printf("LEFT\n");
//			me->gPadHighlight = ERASE;
			erasePadHighlight(me->padSelect);
			switch(me->padSelect){
			case(1):{ //TL
				me->padSelect=2;
				break;
			}
			case(2):{ //TR
				me->padSelect=1;
				break;
			}
			case(3):{ //BL
				me->padSelect=4;
				break;
			}
			case(4):{ //BR
				me->padSelect=3;
				break;
			}
			}
			me->gPadHighlight = DRAW;
			return Q_HANDLED();
		}
		case RIGHT:{
//			printf("RIGHT\n");
//			me->gPadHighlight = ERASE;
			erasePadHighlight(me->padSelect);
			switch(me->padSelect){
			case(1):{ //TL
				me->padSelect=2;
				break;
			}
			case(2):{ //TR
				me->padSelect=1;
				break;
			}
			case(3):{ //BL
				me->padSelect=4;
				break;
			}
			case(4):{ //BR
				me->padSelect=3;
				break;
			}
			}
			me->gPadHighlight = DRAW;
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
//			me->gPadText = ERASE;
//			erasePadText();
//			erasePadHighlight(me->padSelect);
			drawBackground();
//			me->gPadHighlight = ERASE;
			return Q_HANDLED();
		}

	}

	return Q_SUPER(&SM_state_on);
}


QState SM_state_soundSelect(SM *me) {
	switch (Q_SIG(me)) {
			case Q_ENTRY_SIG: {
				printf("soundSelect\n");
				me->gSoundSelectText = DRAW;
				me->gSounds=DRAW;
				return Q_HANDLED();
			}

			case Q_INIT_SIG: {
				return Q_HANDLED();
			}
			case BUTTON:{
				switch(me->padSelect){
				case(1):{//red
//					printf("uart_red: %c\n",uart_red[0]);
					strcpy(uart_red,uart_signal[me->hover]);
//					printf("uart_red: %c\n",uart_red[0]);
					break;
				}
				case(2):{//green
					strcpy(uart_green,uart_signal[me->hover]);
					break;
				}
				case(3):{//yellow
					strcpy(uart_yellow,uart_signal[me->hover]);
					break;
				}
				case(4):{//blue
					strcpy(uart_blue,uart_signal[me->hover]);
					break;
				}
				}
				return Q_TRAN(&SM_state_main);
			}
			case UP:{
				me->hover=(me->hover-1)%20;
				if(me->hover==-1){
					me->hover=19;
				}
				Chip_UART_SendRB(UART_SELECTION, &txring, uart_signal[me->hover], sizeof(uart_signal[me->hover])-3);
				me->gSounds=DRAW;
				return Q_HANDLED();
			}
			case DOWN:{
				me->hover=(me->hover+1)%20;
				Chip_UART_SendRB(UART_SELECTION, &txring, uart_signal[me->hover], sizeof(uart_signal[me->hover])-3);
				me->gSounds=DRAW;
				return Q_HANDLED();
			}
			case Q_EXIT_SIG: {
				drawBackground();
				return Q_HANDLED();
			}

	}
	return Q_SUPER(&SM_state_on);
}
