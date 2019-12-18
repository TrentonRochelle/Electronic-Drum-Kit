/*
 * lcd.h
 *
 *  Created on: Oct 21, 2015
 *      Author: atlantis
 */

/*
  UTFT.h - Multi-Platform library support for Color TFT LCD Boards
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

#ifndef LCD_H_
#define LCD_H_
#include "stdint.h"
#include "board.h"
#define GPIO_CLEAR_PIN		20				// The pin used for clear
#define GPIO_CLEAR_PORT		GPIOINT_PORT0	// The port for the clear pin

#define LCD_DC_PIN			19				// The pin used for LCD_DC
#define LCD_DC_PORT			GPIOINT_PORT0	// The port for the LCD_DC



#define swap(type, i, j)        {type t = i; i = j; j = t;}

#define DISP_X_SIZE     239
#define DISP_Y_SIZE     329



typedef struct _current_font
{
	uint8_t* font;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t offset;
    uint8_t numchars;
};

extern int fch; // Foreground color upper byte
extern int fcl; // Foreground color lower byte
extern int bch; // Background color upper byte
extern int bcl; // Background color lower byte

extern struct _current_font cfont;
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

void LCD_Write_COM(char VL);  
void LCD_Write_DATA(char VL);
void LCD_Write_DATA16(char VH, char VL);

void initLCD(void);
void setXY(int x1, int y1, int x2, int y2);
void setColor(uint8_t r, uint8_t g, uint8_t b);
void setColorBg(uint8_t r, uint8_t g, uint8_t b);
void clrXY(void);
void clrScr(void);
void setCrazyColor(int x, int y);
int noisePattern(int x, int y);
void fillBackground(int x1, int y1, int x2, int y2);
void drawHLine(int x, int y, int l);
void fillRect(int x1, int y1, int x2, int y2);
void fillRectWH(int x, int y, int w, int h);
void setFont(uint8_t* font);
void printChar(uint8_t c, int x, int y);
void lcdPrint(char *st, int x, int y);
void lcdPrint2(char *st, int x, int y);
void drawBackground();
int noisePattern(int x, int y);
int spiralPattern(int x, int y);
void setCrazyColor(int x, int y);
void fillBackground(int x1, int y1, int x2, int y2);
void setCrazyColorBg(int x, int y);
void drawSmashPadText(int color);
void eraseSmashPadText();
void drawPadText();
void erasePadText();
void drawPadTL();
void erasePadTL();
void drawPadTL_unpressed();
void drawPadTL_pressed();
void drawPadTR();
void drawPadTR_unpressed();
void drawPadTR_pressed();
void erasePadTR();
void drawPadBL();
void drawPadBL_unpressed();
void drawPadBL_pressed();
void erasePadBL();
void drawPadBR();
void drawPadBR_unpressed();
void drawPadBR_pressed();
void erasePadBR();
void drawPadHighlight(int padselect);
void erasePadHighlight(int padselect);
void drawSoundSelectText();
void eraseSoundSelectText();
void drawSounds(int index);
void eraseSounds();



#endif /* LCD_H_ */
