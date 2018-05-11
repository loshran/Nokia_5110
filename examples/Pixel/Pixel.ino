#include "Nokia_5110.h"

#define RST 7
#define CE 6
#define DC 8
#define DIN 11
#define CLK 13

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);


void setup() 
{
    /**
     * Note: if instead of text being shown on the display, all the segments are on, you may need to decrease contrast value.
     */
    //lcd.setContrast(60); // 60 is the default value set by the driver
    
    lcd.print("Please Wait ...");
    delay(1000);
    lcd.clear();
    //lcd.Pixel(pos_X, pos_Y) //pos_X and pos_Y in pixel size like 0<=X<=83 and 0<=Y<=47
    lcd.Pixel(1,1);
	  lcd.Pixel(50,47);
	  lcd.Pixel(83,0);
	  delay(1000);
}

void loop() {
	//lcd.OscilloScope(dataport, size_X, size_Y, pos_X, pos_Y, metric_time, MAX_size_bit_signal)
  // dataport is port of ADC
  //size_X, size_Y, pos_X, pos_Y in pixel size like 0<=X<=83 and 0<=Y<=47
  // metric_time is counts of circle
  // MAX_size_bit_signal is max size your signal in ADC bits
  lcd.OscilloScope(A0,50,24,10,23,100,150);
  //lcd.X_line(pos_X, pos_Y,_length);
  lcd.X_line(10,24,50);
  lcd.X_line(10,47,50);
}
