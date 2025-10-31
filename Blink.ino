#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Replace 0x27 with your LCD's I2C address from the scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  lcd.init();          // initialize the lcd 
  lcd.backlight();     // turn on backlight
  lcd.print("Hello STM32");
}

void loop() {
  // your code here
}
