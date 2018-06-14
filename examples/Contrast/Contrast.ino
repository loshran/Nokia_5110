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
    lcd.setContrast(60); // 60 is the default value set by the driver
}

void loop() {
    lcd.clear();
    lcd.println("`setContrast` method lets you change the LCD's Contrast.");
    delay(8000);

    lcd.clear();
    lcd.println("It accepts a value between 0 and 127 (0 >= value <= 127)");
    delay(8000);
}
