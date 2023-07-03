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
    // if (horizontal) {
    //     // set the X coordinates
    //     WriteReg(0x2A);
    //     WriteData_Byte((Xstart+20) >> 8);
    //     WriteData_Byte(Xstart+20);
    //     WriteData_Byte((Xend+20)>> 8);
    //     WriteData_Byte(Xend+20);
        
    //     // set the Y coordinates
    //     WriteReg(0x2B);
    //     WriteData_Byte(Ystart >> 8);
    //     WriteData_Byte(Ystart);
    //     WriteData_Byte((Yend) >> 8);
    //     WriteData_Byte(Yend);
    // }
    // else {
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
    // }

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

/******************************************************************************
  function: Select Image Rotate
  parameter:
    Rotate   :   0,90,180,270
******************************************************************************/
void st7789v2::SetRotate(uint16_t Rotate)
{
  if (Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
    Debug("Set image Rotate %d\r\n", Rotate);
    ROTA = Rotate;
  } else {
    Debug("rotate = 0, 90, 180, 270\r\n");
    //  exit(0);
  }
}

/******************************************************************************
  function: Select Image mirror
  parameter:
    mirror   :       Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void st7789v2::SetMirroring(uint8_t mirror)
{
  if (mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL ||
      mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
    Debug("mirror image x:%s, y:%s\r\n", (mirror & 0x01) ? "mirror" : "none", ((mirror >> 1) & 0x01) ? "mirror" : "none");
    MIRR = mirror;
  } else {
    Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
    //exit(0);
  }
}

/******************************************************************************
  function: Draw Pixels
  parameter:
    Xpoint  :   At point X
    Ypoint  :   At point Y
    Color   :   Painted colors
******************************************************************************/
void st7789v2::SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color)
{
    // if (Xpoint > LCD_WIDTH || Ypoint > LCD_HEIGHT) {
    //     //Debug("Exceeding display boundaries\r\n");
    //     return;
    // }
    uint16_t X, Y;

    switch (ROTA) {
        case ROTATE_0:
          X = Xpoint;
          Y = Ypoint;
          break;
        case ROTATE_90:
          X = LCD_WIDTH - Ypoint - 1;
          Y = Xpoint;
          break;
        case ROTATE_180:
            X = LCD_WIDTH - Xpoint - 1;
            Y = LCD_HEIGHT - Ypoint - 1;
            break;
        case ROTATE_270:
            X = Ypoint;
            Y = LCD_HEIGHT - Xpoint - 1;
            break;
        default:
            return;
    }

    switch (MIRR) {
        case MIRROR_NONE:
            break;
        case MIRROR_HORIZONTAL:
            X = LCD_WIDTH - X - 1;
            break;
        case MIRROR_VERTICAL:
            Y = LCD_HEIGHT - Y - 1;
            break;
        case MIRROR_ORIGIN:
            X = LCD_WIDTH - X - 1;
            Y = LCD_HEIGHT - Y - 1;
            break;
        default:
            return;
    }

    // printf("x = %d, y = %d\r\n", X, Y);
    if (X > LCD_WIDTH || Y > LCD_HEIGHT) {
        //Debug("Exceeding display boundaries\r\n");
        return;
    }

    // UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
    SetUWORD(X, Y, Color);
}

/******************************************************************************
function:	Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		:   The Xpoint coordinate of the point
    Ypoint		:   The Ypoint coordinate of the point
    Color		:   Set color
    Dot_Pixel	:	point size
******************************************************************************/
void st7789v2::DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
                      DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay)
{
    // if (Xpoint > LCD_WIDTH || Ypoint > LCD_HEIGHT) {
    //     Debug("DrawPoint Input exceeds the normal display range\r\n");
    //     return;
    // }

    int16_t XDir_Num , YDir_Num;
    if (Dot_FillWay == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2*Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function:	Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
******************************************************************************/
void st7789v2::DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    // if (Xstart > LCD_WIDTH || Ystart > LCD_HEIGHT ||
    //     Xend > LCD_WIDTH || Yend > LCD_HEIGHT) {
    //     Debug("DrawLine Input exceeds the normal display range\r\n");
    //     return;
    // }

    uint16_t Xpoint = Xstart;
    uint16_t Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
            DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function:	Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Filled : Whether it is filled--- 1 solid 0：empty
******************************************************************************/
void st7789v2::DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Filled )
{
    // if (Xstart > LCD_WIDTH || Ystart > LCD_HEIGHT ||
    //     Xend > LCD_WIDTH || Yend > LCD_HEIGHT) {
    //     Debug("Input exceeds the normal display range\r\n");
    //     return;
    // }

    if (Filled) {
        uint16_t Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            DrawLine(Xstart, Ypoint, Xend, Ypoint, Color ,Line_width, LINE_STYLE_SOLID);
        }
    } else {
        DrawLine(Xstart, Ystart, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
        DrawLine(Xstart, Ystart, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
        DrawLine(Xend, Yend, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
        DrawLine(Xend, Yend, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function:	Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Filled    : Whether it is filled: 1 filling 0：Do not
******************************************************************************/
void st7789v2::DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    // if (X_Center > LCD_WIDTH || Y_Center >= LCD_HEIGHT) {
    //     Debug("DrawCircle Input exceeds the normal display range\r\n");
    //     return;
    // }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}

/******************************************************************************
  function: Show English characters
  parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
void st7789v2::DrawChar(int16_t Xpoint, int16_t Ypoint, const char Acsii_Char, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground)
{
    int16_t Page, Column;

    // if (Xpoint > LCD_WIDTH || Ypoint > LCD_HEIGHT) {
    //     Debug("Paint_DrawChar Input exceeds the normal display range\r\n");
    //     return;
    // }
    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for ( Page = 0; Page < Font->Height; Page ++ ) {
        for ( Column = 0; Column < Font->Width; Column ++ ) {

        //To determine whether the font background color and screen background color is consistent
        if (WHITE == Color_Background) { //this process is to speed up the scan
            if (pgm_read_byte(ptr) & (0x80 >> (Column % 8)))
            SetPixel (Xpoint + Column, Ypoint + Page, Color_Foreground );
        } else {
            if (pgm_read_byte(ptr) & (0x80 >> (Column % 8))) {
            SetPixel (Xpoint + Column, Ypoint + Page, Color_Foreground );
            } else {
            SetPixel (Xpoint + Column, Ypoint + Page, Color_Background );
            }
        }
        //One pixel is 8 bits
        if (Column % 8 == 7) {
            ptr++;
        }
        }/* Write a line */
        if (Font->Width % 8 != 0) {
        ptr++;
        }
    }/* Write all */
}

/******************************************************************************
  function: Display the string
  parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
void st7789v2::DrawString_EN(int16_t Xstart, int16_t Ystart, const char * pString, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground)
{
    int16_t Xpoint = Xstart;
    int16_t Ypoint = Ystart;

    // if (Xstart > LCD_WIDTH || Ystart > LCD_HEIGHT) {
    //     Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
    //     return;
    // }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > LCD_WIDTH) {
        Xpoint = Xstart;
        Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height) > LCD_HEIGHT) {
        Xpoint = Xstart;
        Ypoint = Ystart;
        }
        DrawChar(Xpoint, Ypoint, * pString, Font, Color_Background, Color_Foreground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}

/******************************************************************************
  function: Display nummber
  parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
#define  ARRAY_LEN 50
void st7789v2::DrawNum(int16_t Xpoint, int16_t Ypoint, int32_t Nummber, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground )
{
    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;

    // if (Xpoint > LCD_WIDTH || Ypoint > LCD_HEIGHT) {
    //     //Debug("Paint_DisNum Input exceeds the normal display range\r\n");
    //     return;
    // }

    //Converts a number to a string
    do{
        Num_Array[Num_Bit] = Nummber % 10 + '0';
        Num_Bit++;
        Nummber /= 10;
    }while (Nummber);

    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display float number
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The float data that you want to display
	Decimal_Point	 : Show decimal places
    Font             ：A structure pointer that displays a character size
    Color            : Select the background color of the English character
******************************************************************************/
void st7789v2::DrawFloatNum(int16_t Xpoint, int16_t Ypoint, double Nummber, int8_t Decimal_Point, sFONT* Font, int16_t Color_Background, int16_t Color_Foreground)
{
    char Str[ARRAY_LEN] = {0};
    dtostrf(Nummber,0,Decimal_Point+2,Str);
    char * pStr= (char *)malloc((strlen(Str))*sizeof(char));
    memcpy(pStr,Str,(strlen(Str)-2));
    * (pStr+strlen(Str)-1)='\0';
    if((*(pStr+strlen(Str)-3))=='.')
    {
        *(pStr+strlen(Str)-3)='\0';
    }
    //show
    DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Foreground, Color_Background);
    free(pStr);
    pStr=NULL;
}

/******************************************************************************
  function: Display image
  parameter:
    image            ：Image start address
    xStart           : X starting coordinates
    yStart           : Y starting coordinates
    xEnd             ：Image width
    yEnd             : Image height
******************************************************************************/
void st7789v2::DrawImage(const unsigned char *image, int16_t xStart, int16_t yStart, int16_t W_Image, int16_t H_Image)
{
  int i, j;
  for (j = 0; j < H_Image; j++) {
    for (i = 0; i < W_Image; i++) {
    //   if (xStart + i < LCD_WIDTH  &&  yStart + j < LCD_HEIGHT) //Exceeded part does not display
        SetPixel(xStart + i, yStart + j, (pgm_read_byte(image + j * W_Image * 2 + i * 2 + 1)) << 8 | (pgm_read_byte(image + j * W_Image * 2 + i * 2)));
      //Using arrays is a property of sequential storage, accessing the original array by algorithm
      //j*W_Image*2          Y offset
      //i*2                  X offset
      //pgm_read_byte()
    }
  }
}
