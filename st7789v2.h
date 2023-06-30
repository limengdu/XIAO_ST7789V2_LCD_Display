#ifndef __ST7789V2_H__
#define __ST7789V2_H__

#include <Arduino.h>

#define RST_PIN 1     // D0
#define DC_PIN  2     // D1
#define BL_PIN  3     // D2
#define CS_PIN  4     // D3

#define LCD_WIDTH   240 //LCD width
#define LCD_HEIGHT  280 //LCD height

class st7789v2{
    protected:
        uint8_t spi_cs, spi_dc, spi_rst, spi_bl;
    private:
        void LCD_WriteReg(uint8_t data);
        void LCD_WriteData_Byte(uint8_t data);
        void LCD_WriteData_Word(uint16_t data);
    public:
        bool horizontal = false;
        void LCD_Init(uint8_t cs = CS_PIN, uint8_t dc = DC_PIN, uint8_t rst = RST_PIN, uint8_t bl = BL_PIN);
        void LCD_SetBacklight(uint16_t Value);
        void LCD_Reset(void);
        void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
        void LCD_Clear(uint16_t Color);

};





#endif