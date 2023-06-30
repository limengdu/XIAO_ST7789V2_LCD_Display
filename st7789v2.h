#ifndef __ST7789V2_H__
#define __ST7789V2_H__

#include <Arduino.h>

#define RST_PIN 1     // D0
#define DC_PIN  2     // D1
#define BL_PIN  3     // D2
#define CS_PIN  4     // D3

#define LCD_WIDTH   240 //LCD width
#define LCD_HEIGHT  280 //LCD height

/**
 * image color
**/
#define WHITE         0xFFFF
#define BLACK         0x0000    
#define BLUE          0x001F  
#define BRED          0XF81F
#define GRED          0XFFE0
#define GBLUE         0X07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define GREEN         0x07E0
#define CYAN          0x7FFF
#define YELLOW        0xFFE0
#define BROWN         0XBC40 
#define BRRED         0XFC07 
#define GRAY          0X8430 
#define DARKBLUE      0X01CF  
#define LIGHTBLUE     0X7D7C   
#define GRAYBLUE      0X5458 
#define LIGHTGREEN    0X841F 
#define LGRAY         0XC618 
#define LGRAYBLUE     0XA651
#define LBBLUE        0X2B12 

class st7789v2{
    protected:
        uint8_t spi_cs, spi_dc, spi_rst, spi_bl;
    private:
        void WriteReg(uint8_t data);
        void WriteData_Byte(uint8_t data);
        void WriteData_Word(uint16_t data);
    public:
        bool horizontal = false;
        void Init(uint8_t cs = CS_PIN, uint8_t dc = DC_PIN, uint8_t rst = RST_PIN, uint8_t bl = BL_PIN);
        void SetBacklight(uint16_t Value);
        void Reset(void);
        void SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
        void Clear(uint16_t Color);
        void ClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color);
        void SetWindowColor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color);
        void SetUWORD(uint16_t x, uint16_t y, uint16_t Color);

};





#endif