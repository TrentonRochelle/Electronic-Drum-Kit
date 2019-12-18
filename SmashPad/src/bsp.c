#include "qpn_port.h"
#include "qepn.h"
#include "lcd.h"
#include "statemachine.h"
// Define functions for QPNano
int data = 0;
//uint16_t readValue;
//int twice = 0;
int yellowHit=0;
int blueHit=0;
int greenHit=0;
int redHit=0;
int yellowOut=0;
int blueOut=0;
int greenOut=0;
int redOut=0;
int smashed=0;
//int counter=0;
int y;
int g;
int b;
int r;
void QF_onStartup() {
	//xil_printf("on startup\n");
//	QF_INT_UNLOCK();
}

void QF_onIdle() {
//	QF_INT_UNLOCK();
	y,g,b,r = 0,0,0,0;
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH0, ADC_DR_DONE_STAT) != SET) {}
	data=0;
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &data);
	if(data>2500){
//		twice++;
//		valid=true;
		yellowHit=1;
//		SMASHYELLOW();
	}
	if(data<500 && yellowHit==1){
		yellowHit=0;
		yellowOut=1;
	}
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, DISABLE);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH1, ENABLE);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH1, ADC_DR_DONE_STAT) != SET) {}
	data=0;
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH1, &data);
	if(data>1500){
//		twice++;
//		valid=true;
		greenHit=1;
//		SMASHGREEN();
	}
	if(data<500 && greenHit==1){
		greenHit=0;
		greenOut=1;
	}
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH1, DISABLE);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH2, ENABLE);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH2, ADC_DR_DONE_STAT) != SET) {}
	data=0;
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH2, &data);
	if(data>1500){
//		twice++;
//		valid=true;
		blueHit=1;
//		printf("greenHit: %d\n",greenHit);
//		printf("redHit: %d\n",redHit);
//		printf("yellowHit: %d\n",yellowHit);
//		greenHit=0;
//		redHit=0;
//		yellowHit=0;
//		SMASHBLUE();
	}
	if(data<500 && blueHit==1){
		blueHit=0;
		blueOut=1;
	}
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH2, DISABLE);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH3, ENABLE);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH3, ADC_DR_DONE_STAT) != SET) {}
	data=0;
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH3, &data);
	if(data>500){
//		twice++;
//		valid=true;
		redHit=1;
//		SMASHRED();
	}
	if(data<500 && redHit==1){
		redHit=0;
		redOut=1;
	}
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH3, DISABLE);
//	if(counter<2){
//		yellowHit=0;
//		greenHit=0;
//		blueHit=0;
//		redHit=0;
//		counter++;
//	}
	if((yellowOut==1) || (greenOut==1) || (blueOut==1) || (redOut==1)){
		y = yellowOut;
		g = greenOut;
		b = blueOut;
		r = redOut;
//		yellowHit=0;
//		greenHit=0;
//		blueHit=0;
//		redHit=0;
		yellowOut=0;
		greenOut=0;
		blueOut=0;
		redOut=0;
//		counter = 0;
		SMASH(r,g,y,b);
	}

//	if(yellowHit==2){
//		SMASHYELLOW();
//		yellowHit=0;
//
//	}
//	if(greenHit==2){
//		printf("yellowHit: %d\n",yellowHit);
//		printf("redHit: %d\n",redHit);
//		SMASHGREEN();
//		greenHit=0;
//	}
//	if(blueHit==2){
//		SMASHBLUE();
//		blueHit=0;
//	}
//	if(redHit==2){
//		SMASHRED();
//		redHit=0;
//	}


	switch(Main_SM.gBackground) {
	case NOTHING:
		break;
	case DRAW:
		drawBackground();
		Main_SM.gBackground = NOTHING;
		break;
	case ERASE:
		Main_SM.gBackground = NOTHING;
		break;
	}

	switch(Main_SM.gSmashPadText){
	case NOTHING:
		break;
	case DRAW:
		drawSmashPadText(Main_SM.smashColor);
		Main_SM.gSmashPadText = NOTHING;
		break;
	case ERASE:
		eraseSmashPadText();
		Main_SM.gSmashPadText = NOTHING;
		break;
	}

	switch(Main_SM.gPadText){
	case NOTHING:
		break;
	case DRAW:
		drawPadText();
		Main_SM.gPadText = NOTHING;
		break;
	case ERASE:
		erasePadText();
		Main_SM.gPadText = NOTHING;
		break;
	}

	switch(Main_SM.gPadTL){
	case NOTHING:
		break;
	case DRAW:
		drawPadTL();
		Main_SM.gPadTL = NOTHING;
		break;
	case ERASE:
		erasePadTL();
		Main_SM.gPadTL = NOTHING;
		break;
	}
	switch(Main_SM.gPadTL_unpressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadTL_unpressed();
		Main_SM.gPadTL_unpressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadTL_unpressed = NOTHING;
		break;
	}
	switch(Main_SM.gPadTL_pressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadTL_pressed();
		Main_SM.gPadTL_pressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadTL_pressed = NOTHING;
		break;
	}

	switch(Main_SM.gPadTR){
	case NOTHING:
		break;
	case DRAW:
		drawPadTR();
		Main_SM.gPadTR = NOTHING;
		break;
	case ERASE:
		erasePadTL();
		Main_SM.gPadTR = NOTHING;
		break;
	}
	switch(Main_SM.gPadTR_unpressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadTR_unpressed();
		Main_SM.gPadTR_unpressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadTR_unpressed = NOTHING;
		break;
	}
	switch(Main_SM.gPadTR_pressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadTR_pressed();
		Main_SM.gPadTR_pressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadTR_pressed = NOTHING;
		break;
	}

	switch(Main_SM.gPadBL){
	case NOTHING:
		break;
	case DRAW:
		drawPadBL();
		Main_SM.gPadBL = NOTHING;
		break;
	case ERASE:
		erasePadBL();
		Main_SM.gPadBL = NOTHING;
		break;
	}

	switch(Main_SM.gPadBL_unpressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadBL_unpressed();
		Main_SM.gPadBL_unpressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadBL_unpressed = NOTHING;
		break;
	}
	switch(Main_SM.gPadBL_pressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadBL_pressed();
		Main_SM.gPadBL_pressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadBL_pressed = NOTHING;
		break;
	}

	switch(Main_SM.gPadBR){
	case NOTHING:
		break;
	case DRAW:
		drawPadBR();
		Main_SM.gPadBR = NOTHING;
		break;
	case ERASE:
		erasePadTL();
		Main_SM.gPadBR = NOTHING;
		break;
	}
	switch(Main_SM.gPadBR_unpressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadBR_unpressed();
		Main_SM.gPadBR_unpressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadBR_unpressed = NOTHING;
		break;
	}
	switch(Main_SM.gPadBR_pressed){
	case NOTHING:
		break;
	case DRAW:
		drawPadBR_pressed();
		Main_SM.gPadBR_pressed = NOTHING;
		break;
	case ERASE:
//		erasePadTL();
		Main_SM.gPadBR_pressed = NOTHING;
		break;
	}

	switch(Main_SM.gPadHighlight){
	case NOTHING:
		break;
	case DRAW:
		drawPadHighlight(Main_SM.padSelect);
		Main_SM.gPadHighlight = NOTHING;
		break;
	case ERASE:
		erasePadHighlight(Main_SM.padSelect);
		Main_SM.gPadHighlight = NOTHING;
		break;
	}
	switch(Main_SM.gSoundSelectText){
	case NOTHING:
		break;
	case DRAW:
		drawSoundSelectText();
		Main_SM.gSoundSelectText = NOTHING;
		break;
	case ERASE:
		eraseSoundSelectText();
		Main_SM.gSoundSelectText = NOTHING;
		break;
	}
	switch(Main_SM.gSounds){
	case NOTHING:
		break;
	case DRAW:
		eraseSounds();
		drawSounds(Main_SM.hover);
		Main_SM.gSounds = NOTHING;
		break;
	case ERASE:
		Main_SM.gSoundSelectText = NOTHING;
		break;
	}
}

void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
//    QF_INT_LOCK();
    for (;;) {
    }
}
