#include "Arduino.h"
#include "Nokia_5110.h"
#include "Font.h"
#include "Symbols.h"
#include "SPI.h"

Nokia_5110::Nokia_5110(int8_t RST, int8_t CE, int8_t DC){
	
  _DIN = -1;
  _CLK = -1;
  _DC = DC;
  _RST = RST;
  _CE = CE;
}

Nokia_5110::Nokia_5110(int8_t RST, int8_t CE, int8_t DC, int8_t DIN, int8_t CLK){
	
    _RST = RST;
    _CE = CE;
    _DC = DC;
    _DIN = DIN;
    _CLK = CLK;
}

void Nokia_5110::begin() {
  if (ifHardwareSPI()) {
    // Setup hardware SPI
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
  }
  else {
    // Setup software SPI
    pinMode(_DIN, OUTPUT);
    pinMode(_CLK, OUTPUT);
  }

  pinMode(_DC, OUTPUT);
  if (_RST > 0)
      pinMode(_RST, OUTPUT);
  if (_CE > 0)
      pinMode(_CE, OUTPUT);
  _cursor = Cursor();
  reset();
  clear();
  setDisplayMode(Display_Mode::NORMAL);
  setBiasSystem(Mux_Rate::FORTY);
  setContrast(60);
}

bool Nokia_5110::ifHardwareSPI() {
  return (_DIN == -1 && _CLK == -1);
}

void Nokia_5110::startTransmission(){
    digitalWrite(_CE, LOW);
}

void Nokia_5110::endTransmission(){
    digitalWrite(_CE, HIGH);
}

void Nokia_5110::transmitInformation(byte information){
	
	if (ifHardwareSPI()) {
    // Hardware SPI
	startTransmission();
    SPI.transfer(information);
	endTransmission();
	}
	else {
    startTransmission();
	shiftOut(_DIN, _CLK, MSBFIRST, information);
    endTransmission();
	}
}

void Nokia_5110::execute(byte command){
    initializeForSendingCommand();
    transmitInformation(command);
}

void Nokia_5110::initializeForSendingCommand(){
    digitalWrite(_DC, LOW);
}

/**
 * @param contrast could be 0 to 127
 */
void Nokia_5110::setContrast(unsigned short value){
    if(value > 127)
        return;

    extendedInstruction();
    
    const unsigned short leastValue = 128;
    execute(byte(leastValue + value));
}

void Nokia_5110::extendedInstruction(){
    execute(0x21);
}

void Nokia_5110::basicInstruction(){
    execute(0x20);
}

/**
 * Temperature Coefficient value could be one of 0, 1, 2 or 3;
 */
void Nokia_5110::setTemperatureCoefficient(unsigned short value){

    if(value > 3)
        return;

    extendedInstruction();

    const unsigned short leastValue = 4;
    execute(byte(leastValue + value));
}

void Nokia_5110::reset(){
    digitalWrite(_RST, LOW);
	delay(500);
    digitalWrite(_RST, HIGH);
}

void Nokia_5110::initializeForSendingData(){
    digitalWrite(_DC, HIGH);
}

void Nokia_5110::makeEnoughSpaceForPrinting(unsigned short int newCharacterLength){
    if((newCharacterLength + _cursor.getPosition().x) < 83)
        return;

    _cursor.moveYAxis(1);
    setCursor(_cursor.getPosition().x, _cursor.getPosition().y);
    initializeForSendingData();
}

void Nokia_5110::_print(uint8_t charAsciiCode){

    if (charAsciiCode == 0x0a){ // \n for jumping to the beginning of a new line.
        _cursor.moveYAxis(1);
        return;
    }

    setCursor(_cursor.getPosition().x, _cursor.getPosition().y);

    initializeForSendingData();
    
    character fontData = findCorrespondingByte(charAsciiCode);
    makeEnoughSpaceForPrinting(fontData.definition_total_bytes);

    for(unsigned int i = 0; i < fontData.definition_total_bytes; i++){
        transmitInformation(fontData.definition[i]);
    }

    transmitInformation(0x0); // add an empty line after each chars
    _cursor.moveXAxis(fontData.definition_total_bytes + 1);
}

void Nokia_5110::setCursor(position x, position y){
    _cursor.setCursor(x, y);

    basicInstruction();

    //set x position
    unsigned short int leastXPositionValue = 128;
    execute(byte(leastXPositionValue + x));


    //set y position
    unsigned short int leastYPositionValue = 64;
    execute(byte(leastYPositionValue + y));
}

void Nokia_5110::clear(){
    initializeForSendingData();

    int i = 504;
    while(i >= 0){
        transmitInformation(0x0);
        i--;
    }

    setCursor(0, 0);
}

void Nokia_5110::clear(position inRow, position fromColumn, position toColumn){
    // toColumn has to be more than from Column, otherwise flip the values :D
    position temp;
    if(fromColumn > toColumn){
        temp       = fromColumn;
        fromColumn = toColumn;
        toColumn   = temp;
    }

    position counter = fromColumn;
    while(counter <= toColumn){
        setCursor(counter, inRow);
        initializeForSendingData();
        transmitInformation(0x0);
        counter++;
    }

    setCursor(fromColumn, inRow);
}

void Nokia_5110::setDisplayMode(display_mode value){
    basicInstruction();
    execute(value);
}

void Nokia_5110::setBiasSystem(mux_rate rate){
    extendedInstruction();
    execute(rate);
}

size_t Nokia_5110::write(uint8_t character) {
    _print(character);
    return 1;
}

void Nokia_5110::PixelBorder(uint8_t x, uint8_t y) {
	if (y>47)
  {
    //y=y-48;
    y=47;
  }
  if (x>83)
  {
    //x=x-84;
    x=83;
  }
}

void Nokia_5110::Pixel(uint8_t x, uint8_t y) {
	PixelBorder(x,y);
	uint8_t y_count = 0;
	while(y>7)
	{
		y_count++;
		y -= 8;
	}
	setCursor(x, y_count);
	uint8_t valp;
	switch(y)
	{
	case 0: valp = 0x01;break;
	case 1: valp = 0x02;break;
	case 2: valp = 0x04;break;
	case 3: valp = 0x08;break;
	case 4: valp = 0x10;break;
	case 5: valp = 0x20;break;
	case 6: valp = 0x40;break;
	case 7: valp = 0x80;break;
	default: setCursor(83,5);valp = 0xff;
	}
	initializeForSendingData();
	transmitInformation(valp);
}

void Nokia_5110::X_line(uint8_t pos_X, uint8_t pos_Y, uint8_t _length) {
	for (uint8_t n=0; n<=_length; n++)
	{
	Pixel(pos_X+n,pos_Y);
	}
}

void Nokia_5110::OscilloScope(uint8_t dataport,uint8_t size_X,uint8_t size_Y,uint8_t pos_X, uint8_t pos_Y, uint16_t metric_time, uint8_t MAX_size_bit_signal)
{
  int val = 0;
  uint8_t start_X = pos_X;
  uint8_t start_Y = pos_Y;
  int8_t ADCbit;
  for(uint16_t n=0; n<metric_time; n++)
  {
    if (start_X > 83) start_X=83;
    else if (start_X > (size_X+pos_X)) start_X=pos_X;
    clearColumn(start_X,start_X+5,1,5);
    ADCbit=analogRead(dataport);
    val=(ADCbit-MAX_size_bit_signal)*(size_Y/1024.0); // MAX_size_bit_signal is coefficient for my signal. It is max size of ADCbit for my signal.
    val=round(val+start_Y); 
    Pixel(start_X,val);
    start_X++;
    }
  start_X=0;
  }
  
void Nokia_5110::clearColumn(uint8_t start_x,uint8_t end_x, uint8_t start_y,uint8_t end_y) {
	for (uint8_t n=start_y; n<=end_y; n++)
	{
		clear(n,start_x,end_x);
	}
}

void Nokia_5110::print_symbol(uint8_t symbol_cod_number) {
	
	setCursor(_cursor.getPosition().x, _cursor.getPosition().y);

    initializeForSendingData();
   
	for(int i=0; i<10; i++) {
		transmitInformation(symbols[symbol_cod_number][i]);
	}
	transmitInformation(0x0); // add an empty line after each chars
	_cursor.moveXAxis(10 + 1); // move cursor to 10+1 pixel forward
}