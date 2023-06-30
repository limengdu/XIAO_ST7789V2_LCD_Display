#include <SPI.h>
#include "st7789v2.h"

/******************************************************************************
function: 
    Common register initialization
******************************************************************************/
void st7789v2::Init(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t bl){

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

    Reset();

    //************* Start Initial Sequence **********// 
    
    WriteReg(0x36);
    if (horizontal)
        WriteData_Byte(0x70);
    else
        WriteData_Byte(0x00);
        
    WriteReg(0x3A);
    WriteData_Byte(0x05);

    WriteReg(0xB2);
    WriteData_Byte(0x0B);
    WriteData_Byte(0x0B);
    WriteData_Byte(0x00);
    WriteData_Byte(0x33);
    WriteData_Byte(0x35);

    WriteReg(0xB7);
    WriteData_Byte(0x11);

    WriteReg(0xBB);
    WriteData_Byte(0x35);

    WriteReg(0xC0);
    WriteData_Byte(0x2C);

    WriteReg(0xC2);
    WriteData_Byte(0x01);

    WriteReg(0xC3);
    WriteData_Byte(0x0D);

    WriteReg(0xC4);
    WriteData_Byte(0x20);

    WriteReg(0xC6);
    WriteData_Byte(0x13);

    WriteReg(0xD0);
    WriteData_Byte(0xA4);
    WriteData_Byte(0xA1);

    WriteReg(0xD6);
    WriteData_Byte(0xA1);

    WriteReg(0xE0);
    WriteData_Byte(0xF0);
    WriteData_Byte(0x06);
    WriteData_Byte(0x0B);
    WriteData_Byte(0x0A);
    WriteData_Byte(0x09);
    WriteData_Byte(0x26);
    WriteData_Byte(0x29);
    WriteData_Byte(0x33);
    WriteData_Byte(0x41);
    WriteData_Byte(0x18);
    WriteData_Byte(0x16);
    WriteData_Byte(0x15);
    WriteData_Byte(0x29);
    WriteData_Byte(0x2D);

    WriteReg(0xE1);
    WriteData_Byte(0xF0);
    WriteData_Byte(0x04);
    WriteData_Byte(0x08);
    WriteData_Byte(0x08);
    WriteData_Byte(0x07);
    WriteData_Byte(0x03);
    WriteData_Byte(0x28);
    WriteData_Byte(0x32);
    WriteData_Byte(0x40);
    WriteData_Byte(0x3B);
    WriteData_Byte(0x19);
    WriteData_Byte(0x18);
    WriteData_Byte(0x2A);
    WriteData_Byte(0x2E);

    WriteReg(0xE4);
    WriteData_Byte(0x25);
    WriteData_Byte(0x00);
    WriteData_Byte(0x00);

    WriteReg(0x21);

    WriteReg(0x11);
    delay(120);
    WriteReg(0x29); 
}

/*******************************************************************************
function:
    Write register address and data
*******************************************************************************/
void st7789v2::WriteReg(uint8_t data) {
    digitalWrite(spi_cs, LOW);
    digitalWrite(spi_dc, LOW);
    SPI.transfer(data);
    digitalWrite(spi_cs, HIGH);
}

void st7789v2::WriteData_Byte(uint8_t data) {
    digitalWrite(spi_cs, LOW);
    digitalWrite(spi_dc, HIGH);
    SPI.transfer(data);
    digitalWrite(spi_cs, HIGH);
}

void st7789v2::WriteData_Word(uint16_t data) {
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
void st7789v2::SetBacklight(uint16_t Value) {
    if(Value > 100)
        Value=100;
    analogWrite(spi_bl, (Value * 2.55));
}

/*******************************************************************************
function:
  Hardware reset
*******************************************************************************/
void st7789v2::Reset(void) {
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
void st7789v2::SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend) {
    if (horizontal) {
        // set the X coordinates
        WriteReg(0x2A);
        WriteData_Byte((Xstart+20) >> 8);
        WriteData_Byte(Xstart+20);
        WriteData_Byte((Xend+20)>> 8);
        WriteData_Byte(Xend+20);
        
        // set the Y coordinates
        WriteReg(0x2B);
        WriteData_Byte(Ystart >> 8);
        WriteData_Byte(Ystart);
        WriteData_Byte((Yend) >> 8);
        WriteData_Byte(Yend);
    }
    else {
        // set the X coordinates
        WriteReg(0x2A);
        WriteData_Byte(Xstart >> 8);
        WriteData_Byte(Xstart);
        WriteData_Byte((Xend) >> 8);
        WriteData_Byte(Xend);
        // set the Y coordinates
        WriteReg(0x2B);
        WriteData_Byte((Ystart+20)>> 8);
        WriteData_Byte(Ystart+20);
        WriteData_Byte((Yend+20)>> 8);
        WriteData_Byte(Yend+20);
    }

    WriteReg(0X2C);
}

/******************************************************************************
function: Clear screen function, refresh the screen to a certain color
parameter :
    Color :   The color you want to clear all the screen
******************************************************************************/
void st7789v2::Clear(uint16_t Color) {
    uint16_t i,j;
    SetCursor(0, 0, LCD_WIDTH, LCD_HEIGHT);
    for(i=0; i < LCD_WIDTH; i++) {
        for(j=0; j < LCD_HEIGHT; j++) {
          WriteData_Word(Color);
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
void st7789v2::ClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color)
{
    uint16_t i,j;
    SetCursor(Xstart, Ystart, Xend, Yend);
    for(i=Ystart; i<Yend; i++) {                                
        for(j=Xstart; j<Xend; j++) {
            WriteData_Word(color);
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
void st7789v2::SetWindowColor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color) {
    SetCursor(Xstart, Ystart, Xend, Yend);
    WriteData_Word(Color);
}

/******************************************************************************
function: Draw a uint16_t
parameter :
    X     :   Set the X coordinate
    Y     :   Set the Y coordinate
    Color :   Set the color
******************************************************************************/
void st7789v2::SetUWORD(uint16_t x, uint16_t y, uint16_t Color) {
    SetCursor(x,y,x,y);
    WriteData_Word(Color);
} 








