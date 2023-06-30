#include <st7789v2.h>
#include "SPI.h"

st7789v2 Display;

void setup() {
  // put your setup code here, to run once:
  Display.LCD_Init();
  Display.LCD_SetBacklight(100);
  Display.LCD_Clear(0xF800);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
