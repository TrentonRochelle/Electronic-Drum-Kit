/*
 * lcd.c
 *
 *  Created on: Oct 21, 2015
 *      Author: atlantis
 */

/*
  UTFT.cpp - Multi-Platform library support for Color TFT LCD Boards
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of 
  supported display modules and controllers started to increase I felt 
  it was time to make a single, universal library as it will be much 
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the 
  demo-code provided by ITead studio (for the ITDB02 modules) and 
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic 
  displays, and will work with both Arduino, chipKit boards and select 
  TI LaunchPads. For a full list of tested display modules and controllers,
  see the document UTFT_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some 
  requirements you must adhere to. These requirements can be found 
  in the document UTFT_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#include "lcd.h"
#include "board.h"
#include "stdio.h"
#include "string.h"
#include "gpio_17xx_40xx.h"
#include "math.h"
//#include "ssp_17xx_40xx.h"

#define LPC_SSP           LPC_SSP2
#define SSP_IRQ           SSP2_IRQn
#define SSPIRQHANDLER     SSP1_IRQHandler

#define BUFFER_SIZE                         (0x40)
#define SSP_DATA_BITS                       (SSP_BITS_8)
#define BACKGROUND 100,100,100
#define YELLOW 255,255,0
#define WHITE 255,255,255
#define RED 255,0,0
#define ORANGE 255,123,0
#define YELLOW 255,255,0
#define BLUE	0,0,255
#define GREEN	0,255,0
#define DARKRED 127,0,0
#define DARKYELLOW 127,127,0
#define DARKBLUE 0,0,127
#define DARKGREEN 0,127,0
#define LIGHTYELLOW 255,255,127
#define LIGHTGREEN 127,255,127
#define LIGHTRED 255,127,127
#define LIGHTBLUE 127,127,255


#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define M_PI 3.14159265358979323846264338327950288
// Global variables
int fch;
int fcl;
int bch;
int bcl;
struct _current_font cfont;


const char *sound[] = {
		"Crash 1",
		"Crash 2",
		"Open Hi-Hat 1",
		"Open Hi-Hat 2",
		"Closed Hi-Hat 1",
		"Closed Hi-Hat 2",
		"Electric Kick",
		"Kick One",
		"Kick Two",
		"Snare One",
		"Snare Two",
		"Taiko Drum",
		"Taiko Stick",
		"Tom 1",
		"Tom 2",
		"Tom 3",
		"Tap",
		"Slap",
		"Mario Coin",
		"Mario Jump"
};




void LCD_Write_COM(char VL) {
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_DC_PORT, LCD_DC_PIN, false);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP2, &VL, 1);
}

/**
 * @brief	Writes data to the LCD
 * @return	Nothing
 */
void LCD_Write_DATA16(char VH, char VL) {
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_DC_PORT, LCD_DC_PIN, true);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP2, &VH, 1);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP2, &VL, 1);

}


// Write 8-bit data to LCD controller
void LCD_Write_DATA(char VL)
{
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_DC_PORT, LCD_DC_PIN, true);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP2, &VL, 1);
}


// Initialize LCD controller
void initLCD(void)
{
    int i;



    // Reset
    LCD_Write_COM(0x01);
    for (i = 0; i < 500000; i++); //Must wait > 5ms


    LCD_Write_COM(0xCB);
    LCD_Write_DATA(0x39);
    LCD_Write_DATA(0x2C);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x34);
    LCD_Write_DATA(0x02);

    LCD_Write_COM(0xCF); 
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0XC1);
    LCD_Write_DATA(0X30);

    LCD_Write_COM(0xE8); 
    LCD_Write_DATA(0x85);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x78);

    LCD_Write_COM(0xEA); 
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
 
    LCD_Write_COM(0xED); 
    LCD_Write_DATA(0x64);
    LCD_Write_DATA(0x03);
    LCD_Write_DATA(0X12);
    LCD_Write_DATA(0X81);

    LCD_Write_COM(0xF7); 
    LCD_Write_DATA(0x20);
  
    LCD_Write_COM(0xC0);   //Power control 
    LCD_Write_DATA(0x23);  //VRH[5:0] 
 
    LCD_Write_COM(0xC1);   //Power control 
    LCD_Write_DATA(0x10);  //SAP[2:0];BT[3:0] 

    LCD_Write_COM(0xC5);   //VCM control 
    LCD_Write_DATA(0x3e);  //Contrast
    LCD_Write_DATA(0x28);
 
    LCD_Write_COM(0xC7);   //VCM control2 
    LCD_Write_DATA(0x86);  //--
 
    LCD_Write_COM(0x36);   // Memory Access Control 
    LCD_Write_DATA(0x48);  

    LCD_Write_COM(0x3A);   
    LCD_Write_DATA(0x55);

    LCD_Write_COM(0xB1);   
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x18);
 
    LCD_Write_COM(0xB6);   // Display Function Control 
    LCD_Write_DATA(0x08);
    LCD_Write_DATA(0x82);
    LCD_Write_DATA(0x27); 

    LCD_Write_COM(0x11);   //Exit Sleep 
    for (i = 0; i < 100000; i++);
                
    LCD_Write_COM(0x29);   //Display on 
    LCD_Write_COM(0x2c); 

    //for (i = 0; i < 100000; i++);

    // Default color and fonts
    fch = 0xFF;
    fcl = 0xFF;
    bch = 0x00;
    bcl = 0x00;
    setFont(BigFont);
}


// Set boundary for drawing
void setXY(int x1, int y1, int x2, int y2)
{
    LCD_Write_COM(0x2A); 
    LCD_Write_DATA(x1 >> 8);
    LCD_Write_DATA(x1);
    LCD_Write_DATA(x2 >> 8);
    LCD_Write_DATA(x2);
    LCD_Write_COM(0x2B); 
    LCD_Write_DATA(y1 >> 8);
    LCD_Write_DATA(y1);
    LCD_Write_DATA(y2 >> 8);
    LCD_Write_DATA(y2);
    LCD_Write_COM(0x2C);
}


// Remove boundry
void clrXY(void)
{
    setXY(0, 0, DISP_X_SIZE, DISP_Y_SIZE);
}


// Set foreground RGB color for next drawing
void setColor(uint8_t r, uint8_t g, uint8_t b)
{
    // 5-bit r, 6-bit g, 5-bit b
    fch = (r & 0x0F8) | g >> 5;
    fcl = (g & 0x1C) << 3 | b >> 3;
}


// Set background RGB color for next drawing
void setColorBg(uint8_t r, uint8_t g, uint8_t b)
{
    // 5-bit r, 6-bit g, 5-bit b
    bch = (r & 0x0F8) | g >> 5;
    bcl = (g & 0x1C) << 3 | b >> 3;
}


// Clear display
void clrScr(void)
{
    // Black screen
    setColor(0, 0, 0);

    fillRect(0, 0, DISP_X_SIZE, DISP_Y_SIZE);
}


// Draw horizontal line
void drawHLine(int x, int y, int l)
{
    int i;

    if (l < 0) {
        l = -l;
        x -= l;
    }

    setXY(x, y, x + l, y);
    for (i = 0; i < l + 1; i++) {
        LCD_Write_DATA16(fch, fcl);
    }

    clrXY();
}


// Fill a rectangular 
void fillRect(int x1, int y1, int x2, int y2)
{
    int i;

    if (x1 > x2)
        swap(int, x1, x2);

    if (y1 > y2)
        swap(int, y1, y2);

    setXY(x1, y1, x2, y2);
    for (i = 0; i < (x2 - x1 + 1) * (y2 - y1 + 1); i++) {
        LCD_Write_DATA16(fch, fcl);
    }

   clrXY();
}

void fillRectWH(int x, int y, int w, int h) {
	setXY(x,y,x+w-1,y+h-1);
	int i;
	for (i=0; i < w*h; i++) {
		LCD_Write_DATA16(fch, fcl);
	}
}

// Select the font used by print() and printChar()
void setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size = font[0];
	cfont.y_size = font[1];
	cfont.offset = font[2];
	cfont.numchars = font[3];
}


// Print a character
void printChar(uint8_t c, int x, int y)
{
    uint8_t ch;
    int i, j,zz, pixelIndex,temp;

    setXY(x, y, x + cfont.x_size - 1,y + cfont.y_size - 1);

//    pixelIndex =
//            (c - cfont.offset) * (cfont.x_size >> 3) * cfont.y_size + 4;
//    for(j = 0; j < (cfont.x_size >> 3) * cfont.y_size; j++) {
//        ch = cfont.font[pixelIndex];
//        for(i = 0; i < 8; i++) {
//            if ((ch & (1 << (7 - i))) != 0){
//                LCD_Write_DATA16(fch, fcl);
//            }
//            else{
////            	setCrazyColorBg(j,i);
//                LCD_Write_DATA16(bch, bcl);
////                LCD_Write_DATA16(fch, fcl);
//
//            }
//        }
//        pixelIndex++;
//    }
    temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
    for(j=0;j<cfont.y_size;j++){
		for (zz=0; zz<(cfont.x_size/8); zz++){
			ch = cfont.font[temp+zz];
			for(i=0;i<8;i++){
				if((ch&(1<<(7-i)))!=0){
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
	//    						setPixel((fch<<8)|fcl);
					LCD_Write_DATA16((((fch<<8)|fcl)>>8),(((fch<<8)|fcl)&0xFF));
				}
			}
		}
		temp+=(cfont.x_size/8);
    }
    clrXY();
}


// Print string
void lcdPrint(char *st, int x, int y)
{
    int i = 0;

    while(*st != '\0')
        printChar(*st++, x + cfont.x_size * i++, y);
}
void lcdPrint2(char *st, int x, int y)
{
    int i = 0;

    while(*st != '\0')
        printChar(*st++, x +  (int)(cfont.x_size/1.5) * i++, y);
}


void drawBackground() {
//	setColor(BACKGROUND);
//	fillRect(0,0,240,320);
	fillBackground(0,0,239,319);
}

int noisePattern(int x, int y) {
	int hash = x*x + y*y*y;
	hash >>= 3;

	return (hash%40) < 6;
}
int spiralPattern(int x, int y) {
	x = LCD_WIDTH/2 - x;
	y = LCD_HEIGHT/2 - y;
	float r = sqrt(x*x+y*y);
	float theta = atan2(y, x);

	return fmod((r - 30*theta), 2*M_PI) < 1;
}

void setCrazyColor(int x, int y) {
	setColor(
		(x+x-y)%256,
		(y+x)%256,
		(y+y-x)%256
	);
}
void setCrazyColorBg(int x, int y) {
	setColorBg(
		(x+y)%256,
		(2*256+y-x)%256,
		(x*y)%256
	);
}
void fillBackground(int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        swap(int, x1, x2);

    if (y1 > y2)
        swap(int, y1, y2);
    setXY(x1, y1, x2, y2);

    int lastColor = 0;
    setColor(0, 255, 0);
    int ytemp;
    int xtemp;
    for (ytemp = y1; ytemp <= y2; ytemp++) {
    	for (xtemp = x1; xtemp <= x2; xtemp++) {

    		setCrazyColor(xtemp, ytemp);
    		LCD_Write_DATA16(fch, fcl);
    		continue;

//    		if (circlePattern(xtemp, ytemp) != diamondPattern(xtemp, ytemp)) {
    		if (noisePattern(xtemp, ytemp)) {
    			if (lastColor != 0) {
    				setColor(0, 255, 0);
    				lastColor = 0;
    			}
    		}
    		else{
    			if (lastColor != 1) {
    				setColor(204, 0, 102);
					lastColor = 1;
				}
    		}
    		LCD_Write_DATA16(fch, fcl);
    	}
    }

   clrXY();
}

void drawSmashPadText(int color){
	setFont(BigFont);
//	setColor(ORANGE);
	setColorBg(BACKGROUND);
//	printf("color: %d\n",color);
	if(color==0){
		setColor(DARKRED);
		lcdPrint("SmashPad!", 120-9*8, 25);
		setColor(RED);
		lcdPrint("SmashPad!", 120-9*8, 35);
		setColor(LIGHTRED);
		lcdPrint("SmashPad!", 120-9*8, 45);
	}
	else if(color==1){
		setColor(DARKGREEN);
		lcdPrint("SmashPad!", 120-9*8, 25);
		setColor(GREEN);
		lcdPrint("SmashPad!", 120-9*8, 35);
		setColor(LIGHTGREEN);
		lcdPrint("SmashPad!", 120-9*8, 45);
	}
	else if(color==2){
		setColor(DARKYELLOW);
		lcdPrint("SmashPad!", 120-9*8, 25);
		setColor(YELLOW);
		lcdPrint("SmashPad!", 120-9*8, 35);
		setColor(LIGHTYELLOW);
		lcdPrint("SmashPad!", 120-9*8, 45);
	}
	else{
		setColor(DARKBLUE);
		lcdPrint("SmashPad!", 120-9*8, 25);
		setColor(BLUE);
		lcdPrint("SmashPad!", 120-9*8, 35);
		setColor(LIGHTBLUE);
		lcdPrint("SmashPad!", 120-9*8, 45);
	}




}

void eraseSmashPadText(){
	setColor(BACKGROUND);
	fillRectWH(120-9*8,35,9*16,16);
}

void drawPadText(){
	setFont(BigFont);
	setColor(WHITE);
	setColorBg(BACKGROUND);
	lcdPrint("Select Pad", 120-10*8, 35);
}

void erasePadText(){
	setColor(BACKGROUND);
	fillRectWH(120-10*8,35,10*16,16);
}



void drawPadHighlight(int padselect){
	setColor(ORANGE);
//	fillRectWH(10,110,100,100);
//	fillRectWH(10,10,100,10);
//	fillRectWH(10,10,10,100);
//	fillRectWH(110,10,10,100);
//	fillRectWH(10,100,100,10);
//
//	int offset1 = [0,80];
//	int offset2 = [120,80];
//	int offset3 = [0,200];
//	int offset4 = [120,200];
	switch(padselect){
	case(1):{
		fillRectWH(10,10+80,100,10);
		fillRectWH(10,10+80,10,100);
		fillRectWH(100,10+80,10,100);
		fillRectWH(10,100+80,100,10);
		break;
	}
	case(2):{
		fillRectWH(10+120,10+80,100,10);
		fillRectWH(10+120,10+80,10,100);
		fillRectWH(100+120,10+80,10,100);
		fillRectWH(10+120,100+80,100,10);
		break;
	}
	case(3):{
		fillRectWH(10,10+200,100,10);
		fillRectWH(10,10+200,10,100);
		fillRectWH(100,10+200,10,100);
		fillRectWH(10,100+200,100,10);
		break;
	}
	case(4):{
		fillRectWH(10+120,10+200,100,10);
		fillRectWH(10+120,10+200,10,100);
		fillRectWH(100+120,10+200,10,100);
		fillRectWH(10+120,100+200,100,10);
		break;
	}
	}
}

void erasePadHighlight(int padselect){
//	setColor(BACKGROUND);
	switch(padselect){
	case(1):{
//		fillRectWH(10,10+80,100,10);
//		fillRectWH(10,10+80,10,100);
//		fillRectWH(100,10+80,10,100);
//		fillRectWH(10,100+80,100,10);
		fillBackground(10,90,109,99);
		fillBackground(10,90,19,189);
		fillBackground(100,90,109,189);
		fillBackground(10,180,109,189);
		break;
	}
	case(2):{
//		fillRectWH(10+120,10+80,100,10);
//		fillRectWH(10+120,10+80,10,100);
//		fillRectWH(100+120,10+80,10,100);
//		fillRectWH(10+120,100+80,100,10);
		fillBackground(130,90,229,99);
		fillBackground(130,90,139,189);
		fillBackground(220,90,229,189);
		fillBackground(130,180,229,189);
		break;
	}
	case(3):{
//		fillRectWH(10,10+200,100,10);
//		fillRectWH(10,10+200,10,100);
//		fillRectWH(100,10+200,10,100);
//		fillRectWH(10,100+200,100,10);
		fillBackground(10,210,109,219);
		fillBackground(10,210,19,309);
		fillBackground(100,210,109,309);
		fillBackground(10,300,109,309);
		break;
	}
	case(4):{
//		fillRectWH(10+120,10+200,100,10);
//		fillRectWH(10+120,10+200,10,100);
//		fillRectWH(100+120,10+200,10,100);
//		fillRectWH(10+120,100+200,100,10);
		fillBackground(130,210,229,219);
		fillBackground(130,210,139,309);
		fillBackground(220,210,229,309);
		fillBackground(130,300,229,309);
		break;
	}
	}
}

void drawPadTL(){
	setColor(RED);
	fillRectWH(20,100,80,80);
}

void erasePadTL(){
	setColor(BACKGROUND);
	fillRectWH(20,80,80,80);
}

void drawPadTR(){
	setColor(GREEN);
	fillRectWH(140,100,80,80);
}

void erasePadTR(){
	setColor(BACKGROUND);
	fillRectWH(140,100,80,80);
}
void drawPadBL(){
	setColor(YELLOW);
	fillRectWH(20,220,80,80);
}
void erasePadBL(){
	setColor(BACKGROUND);
	fillRectWH(20,220,80,80);
}
void drawPadBR(){
	setColor(BLUE);
	fillRectWH(140,220,80,80);
}
void erasePadBR(){
	setColor(BACKGROUND);
	fillRectWH(140,220,80,80);
}

void drawPadTL_unpressed(){
	setColor(RED);
	fillRectWH(20,100,80,10);
	fillRectWH(20,110,10,70);
	setColor(DARKRED);
	fillRectWH(100,110,10,80);
	fillRectWH(30,180,70,10);
}

void drawPadTR_unpressed(){
	setColor(GREEN);
	fillRectWH(140,100,80,10);
	fillRectWH(210,110,10,70);
	setColor(DARKGREEN);
	fillRectWH(130,110,10,80);
	fillRectWH(140,180,70,10);
}

void drawPadBL_unpressed(){
	setColor(YELLOW);
	fillRectWH(20,220,10,80);
	fillRectWH(30,290,70,10);
//	fillRectWH(20,220,80,80);
	setColor(DARKYELLOW);
	fillRectWH(100,210,10,80);
	fillRectWH(30,210,70,10);
}

void drawPadBR_unpressed(){
	setColor(BLUE);
	fillRectWH(210,220,10,80);
	fillRectWH(140,290,70,10);
//	fillRectWH(140,220,80,80);
	setColor(DARKBLUE);
	fillRectWH(130,220,10,70);
	fillRectWH(130,210,80,10);
}



void drawPadTL_pressed(){
//	setColor(BACKGROUND);
//	fillRectWH(20,100,80,10);
//	fillRectWH(20,110,10,70);
	fillBackground(20,100,99,109);
	fillBackground(20,110,29,179);

	setColor(RED);
	fillRectWH(100,110,10,80);
	fillRectWH(30,180,70,10);
}

void drawPadTR_pressed(){
//	setColor(BACKGROUND);
//	fillRectWH(140,100,80,10);
//	fillRectWH(210,110,10,70);
	fillBackground(140,100,219,109);
	fillBackground(210,110,219,179);

	setColor(GREEN);
	fillRectWH(130,110,10,80);
	fillRectWH(140,180,70,10);
}

void drawPadBL_pressed(){
//	setColor(BACKGROUND);
//	fillRectWH(20,220,10,80);
//	fillRectWH(30,290,70,10);
	fillBackground(20,220,29,299);
	fillBackground(30,290,99,299);

	setColor(YELLOW);
	fillRectWH(100,210,10,80);
	fillRectWH(30,210,70,10);
}

void drawPadBR_pressed(){
//	setColor(BACKGROUND);
//	fillRectWH(210,220,10,80);
//	fillRectWH(140,290,70,10);
	fillBackground(210,220,219,299);
	fillBackground(140,290,209,299);

//	fillRectWH(140,220,80,80);
	setColor(BLUE);
	fillRectWH(130,220,10,70);
	fillRectWH(130,210,80,10);
}

void drawSoundSelectText(){
	setFont(BigFont);
	setColor(0,0,0);
	lcdPrint("Select", 120-6*8, 20);
	lcdPrint("Smash Sound", 120-11*8, 35);

}

void eraseSoundSelectText(){
	fillRectWH(120-6*8,20,6*16,16);
	fillRectWH(120-11*8,35,11*16,16);

}

void drawSounds(int index){
	//main @ x=130,y=240
	setFont(BigFont);
	setColor(YELLOW);
	lcdPrint2(sound[index],120-strlen(sound[index])*6,240);
	setColor(ORANGE);
	int i = (index+1)%20;
	lcdPrint2(sound[i],120-strlen(sound[i])*6,280);
	int j = (index-1)%20;
	if(j==-1){
		j=19;
		lcdPrint2(sound[j],120-strlen(sound[j])*6,200);
	}
	else{
		lcdPrint2(sound[j],120-strlen(sound[j])*6,200);

	}




}
void eraseSounds(){
//	fillRectWH(140-15*6,240,6*15,16);
//	fillRectWH(110-15*6,280,6*15,16);
//	fillRectWH(170-15*6,200,6*15,16);
	fillBackground(120-15*6,240,239,256);
	fillBackground(120-15*6,280,239,296);
	fillBackground(120-15*6,200,239,216);

}
