#include "Nokia_5110.h"

#define RST 2
#define CE 3
#define DC 4
#define DIN 5
#define CLK 6

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);


void setup() {
    /**
     * Note: if instead of text being shown on the display, all the segments are on, you may need to decrease contrast value.
     */
    //lcd.setContrast(60); // 60 is the default value set by the driver
    
    lcd.print("Please Wait ...");
    delay(1000);
    lcd.clear();

    lcd.Pixel(1,1);
	lcd.Pixel(50,47);
	lcd.Pixel(83,0);
	delay(1000);
}

void loop() {
	
}
