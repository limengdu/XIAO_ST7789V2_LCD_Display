#include <SPI.h>
#include "st7789v2.h"

/******************************************************************************
function: 
    Common register initialization
******************************************************************************/
void st7789v2::LCD_Init(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t bl){

    spi_cs = cs;
    spi_dc = dc;
    spi_rst = rst;
    spi_bl = bl;

    pinMode(spi_cs, OUTPUT);
    pinMode(spi_rst, OUTPUT);
    pinMode(spi_dc, OUTPUT);
    pinMode(spi_bl, OUTPUT);
    analogWrite(spi_bl, 140);

    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.begin();

    LCD_Reset();

    //************* Start Initial Sequence **********// 
    
    LCD_WriteReg(0x36);
    if (horizontal)
        LCD_WriteData_Byte(0x70);
    else
        LCD_WriteData_Byte(0x00);
        
    LCD_WriteReg(0x3A);
    LCD_WriteData_Byte(0x05);

    LCD_WriteReg(0xB2);
    LCD_WriteData_Byte(0x0B);
    LCD_WriteData_Byte(0x0B);
    LCD_WriteData_Byte(0x00);
    LCD_WriteData_Byte(0x33);
    LCD_WriteData_Byte(0x35);

    LCD_WriteReg(0xB7);
    LCD_WriteData_Byte(0x11);

    LCD_WriteReg(0xBB);
    LCD_WriteData_Byte(0x35);

    LCD_WriteReg(0xC0);
    LCD_WriteData_Byte(0x2C);

    LCD_WriteReg(0xC2);
    LCD_WriteData_Byte(0x01);

    LCD_WriteReg(0xC3);
    LCD_WriteData_Byte(0x0D);

    LCD_WriteReg(0xC4);
    LCD_WriteData_Byte(0x20);

    LCD_WriteReg(0xC6);
    LCD_WriteData_Byte(0x13);

    LCD_WriteReg(0xD0);
    LCD_WriteData_Byte(0xA4);
    LCD_WriteData_Byte(0xA1);

    LCD_WriteReg(0xD6);
    LCD_WriteData_Byte(0xA1);

    LCD_WriteReg(0xE0);
    LCD_WriteData_Byte(0xF0);
    LCD_WriteData_Byte(0x06);
    LCD_WriteData_Byte(0x0B);
    LCD_WriteData_Byte(0x0A);
    LCD_WriteData_Byte(0x09);
    LCD_WriteData_Byte(0x26);
    LCD_WriteData_Byte(0x29);
    LCD_WriteData_Byte(0x33);
    LCD_WriteData_Byte(0x41);
    LCD_WriteData_Byte(0x18);
    LCD_WriteData_Byte(0x16);
    LCD_WriteData_Byte(0x15);
    LCD_WriteData_Byte(0x29);
    LCD_WriteData_Byte(0x2D);

    LCD_WriteReg(0xE1);
    LCD_WriteData_Byte(0xF0);
    LCD_WriteData_Byte(0x04);
    LCD_WriteData_Byte(0x08);
    LCD_WriteData_Byte(0x08);
    LCD_WriteData_Byte(0x07);
    LCD_WriteData_Byte(0x03);
    LCD_WriteData_Byte(0x28);
    LCD_WriteData_Byte(0x32);
    LCD_WriteData_Byte(0x40);
    LCD_WriteData_Byte(0x3B);
    LCD_WriteData_Byte(0x19);
    LCD_WriteData_Byte(0x18);
    LCD_WriteData_Byte(0x2A);
    LCD_WriteData_Byte(0x2E);

    LCD_WriteReg(0xE4);
    LCD_WriteData_Byte(0x25);
    LCD_WriteData_Byte(0x00);
    LCD_WriteData_Byte(0x00);

    LCD_WriteReg(0x21);

    LCD_WriteReg(0x11);
    delay(120);
    LCD_WriteReg(0x29); 
}

/*******************************************************************************
function:
    Write register address and data
*******************************************************************************/
void st7789v2::LCD_WriteReg(uint8_t data) {
    digitalWrite(spi_cs, LOW);
    digitalWrite(spi_dc, LOW);
    SPI.transfer(data);
    digitalWrite(spi_cs, HIGH);
}

void st7789v2::LCD_WriteData_Byte(uint8_t data) {
    digitalWrite(spi_cs, LOW);
    digitalWrite(spi_dc, HIGH);
    SPI.transfer(data);
    digitalWrite(spi_cs, HIGH);
}

void st7789v2::LCD_WriteData_Word(uint16_t data) {
    uint8_t i=(data>>8)&0xff;
    digitalWrite(spi_cs, LOW);
    digitalWrite(spi_dc, HIGH);
    SPI.transfer(i);
    SPI.transfer(data);
    digitalWrite(spi_cs, HIGH);
}

/*******************************************************************************
function:
  Setting backlight
parameter :
    value : Range 0~255   Duty cycle is value/255
*******************************************************************************/
void st7789v2::LCD_SetBacklight(uint16_t Value) {
    if(Value > 100)
        Value=100;
    analogWrite(spi_bl, (Value * 2.55));
}

/*******************************************************************************
function:
  Hardware reset
*******************************************************************************/
void st7789v2::LCD_Reset(void) {
    digitalWrite(spi_cs, LOW);
    delay(20);
    digitalWrite(spi_rst, LOW);
    delay(20);
    digitalWrite(spi_rst, HIGH);
    delay(20);
}

/******************************************************************************
function: Set the cursor position
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinatesen
******************************************************************************/
void st7789v2::LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend) {
    if (horizontal) {
        // set the X coordinates
        LCD_WriteReg(0x2A);
        LCD_WriteData_Byte((Xstart+20) >> 8);
        LCD_WriteData_Byte(Xstart+20);
        LCD_WriteData_Byte((Xend+20)>> 8);
        LCD_WriteData_Byte(Xend+20);
        
        // set the Y coordinates
        LCD_WriteReg(0x2B);
        LCD_WriteData_Byte(Ystart >> 8);
        LCD_WriteData_Byte(Ystart);
        LCD_WriteData_Byte((Yend) >> 8);
        LCD_WriteData_Byte(Yend);
    }
    else {
        // set the X coordinates
        LCD_WriteReg(0x2A);
        LCD_WriteData_Byte(Xstart >> 8);
        LCD_WriteData_Byte(Xstart);
        LCD_WriteData_Byte((Xend) >> 8);
        LCD_WriteData_Byte(Xend);
        // set the Y coordinates
        LCD_WriteReg(0x2B);
        LCD_WriteData_Byte((Ystart+20)>> 8);
        LCD_WriteData_Byte(Ystart+20);
        LCD_WriteData_Byte((Yend+20)>> 8);
        LCD_WriteData_Byte(Yend+20);
    }

    LCD_WriteReg(0X2C);
}

/******************************************************************************
function: Clear screen function, refresh the screen to a certain color
parameter :
    Color :   The color you want to clear all the screen
******************************************************************************/
void st7789v2::LCD_Clear(uint16_t Color) {
    uint16_t i,j;
    LCD_SetCursor(0, 0, LCD_WIDTH, LCD_HEIGHT);
    for(i=0; i < LCD_WIDTH; i++) {
        for(j=0; j < LCD_HEIGHT; j++) {
          LCD_WriteData_Word(Color);
        }
    }
}

/******************************************************************************
function: Refresh a certain area to the same color
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinates
    color :   Set the color
******************************************************************************/
void st7789v2::LCD_ClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color)
{
    uint16_t i,j;
    LCD_SetCursor(Xstart, Ystart, Xend, Yend);
    for(i=Ystart; i<Yend; i++) {                                
        for(j=Xstart; j<Xend; j++) {
            LCD_WriteData_Word(color);
        }
    }
}

/******************************************************************************
function: Set the color of an area
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinates
    Color :   Set the color
******************************************************************************/
void st7789v2::LCD_SetWindowColor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color) {
    LCD_SetCursor(Xstart, Ystart, Xend, Yend);
    LCD_WriteData_Word(Color);
}

/******************************************************************************
function: Draw a uint16_t
parameter :
    X     :   Set the X coordinate
    Y     :   Set the Y coordinate
    Color :   Set the color
******************************************************************************/
void st7789v2::LCD_SetUWORD(uint16_t x, uint16_t y, uint16_t Color) {
    LCD_SetCursor(x,y,x,y);
    LCD_WriteData_Word(Color);
} 








