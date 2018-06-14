#include "Nokia_5110.h"

#define RST 7
#define CE 6
#define DC 8
//#define DIN 11
//#define CLK 13

//Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK); // software SPI
Nokia_5110 lcd = Nokia_5110(RST, CE, DC); // hardware SPI

void setup() {
	lcd.begin();
    /**
     * Note: if instead of text being shown on the display, all the segments are on, you may need to decrease contrast value.
     */
    //lcd.setContrast(60); // 60 is the default value set by the driver
    
    lcd.print("Please Wait ...");
    delay(1000);
    lcd.clear();

    lcd.print("Hi there");
    lcd.println(":D");

    lcd.setCursor(0, 5);
    lcd.println("1 2 3 ...");
    lcd.clear();
}

void loop() {
  lcd.setCursor(50, 0);
  lcd.print_symbol(0x03); //Flash card simbol
  delay(1000);
  }
