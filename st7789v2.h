#ifndef __ST7789V2_H__
#define __ST7789V2_H__

#include <Arduino.h>
#include "stdio.h"
#include "fonts.h"

#define RST_PIN 1     // D0
#define DC_PIN  2     // D1
#define BL_PIN  3     // D2
#define CS_PIN  4     // D3

#define LCD_WIDTH   240 //LCD width
#define LCD_HEIGHT  280 //LCD height

#define DEV_DEBUG 1
#if DEV_DEBUG
  #define Debug(__info,...) printf("Debug : " __info,##__VA_ARGS__)
#else
  #define DEBUG(__info,...)  
#endif

/**
 * The size of the point
**/
typedef enum {
    DOT_PIXEL_1X1  = 1,   // 1 x 1
    DOT_PIXEL_2X2  ,    // 2 X 2
    DOT_PIXEL_3X3  ,    // 3 X 3
    DOT_PIXEL_4X4  ,    // 4 X 4
    DOT_PIXEL_5X5  ,    // 5 X 5
    DOT_PIXEL_6X6  ,    // 6 X 6
    DOT_PIXEL_7X7  ,    // 7 X 7
    DOT_PIXEL_8X8  ,    // 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

/**
 * Point size fill style
**/
typedef enum {
    DOT_FILL_AROUND  = 1,   // dot pixel 1 x 1
    DOT_FILL_RIGHTUP  ,     // dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex

/**
 * Line style, solid or dashed
**/
typedef enum {
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
} LINE_STYLE;

/**
 * Whether the graphic is filled
**/
typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;

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

/**
 * Display rotate
**/
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

#define MIRROR_NONE        0x00
#define MIRROR_HORIZONTAL  0x01
#define MIRROR_VERTICAL    0x02
#define MIRROR_ORIGIN      0x03

class st7789v2{
    protected:
        uint8_t spi_cs, spi_dc, spi_rst, spi_bl;
        uint16_t ROTA = ROTATE_0, MIRR = MIRROR_NONE;
    private:
        void WriteReg(uint8_t data);
        void WriteData_Byte(uint8_t data);
        void WriteData_Word(uint16_t data);
        void DrawChar(int16_t Xpoint, int16_t Ypoint, const char Acsii_Char, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground);


    public:
        // bool horizontal = false;
        uint8_t mirror = MIRROR_NONE;
        void Init(uint8_t cs = CS_PIN, uint8_t dc = DC_PIN, uint8_t rst = RST_PIN, uint8_t bl = BL_PIN);
        void SetBacklight(uint16_t Value);
        void Reset(void);
        void SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
        void Clear(uint16_t Color);
        void ClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color);
        void SetWindowColor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color);
        void SetUWORD(uint16_t x, uint16_t y, uint16_t Color);
        void SetRotate(uint16_t Rotate);
        void SetMirroring(uint8_t mirror);
        void SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color);
        void DrawPoint( uint16_t Xpoint, uint16_t Ypoint, uint16_t Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
        void DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style);
        void DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Filled);
        void DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);
        void DrawString_EN(int16_t Xstart, int16_t Ystart, const char * pString, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground);
        void DrawNum(int16_t Xpoint, int16_t Ypoint, int32_t Nummber, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground);
        void DrawFloatNum(int16_t Xpoint, int16_t Ypoint, double Nummber,  int8_t Decimal_Point, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground);
        void DrawImage(const unsigned char *image, int16_t xStart, int16_t yStart, int16_t W_Image, int16_t H_Image);
};





#endif