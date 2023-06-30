#include <st7789v2.h>
#include "SPI.h"

st7789v2 lcd;

void setup() {
  // put your setup code here, to run once:
  lcd.Init();
  lcd.SetBacklight(100);
  lcd.Clear(RED);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
