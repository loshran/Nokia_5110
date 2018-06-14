#ifndef NOKIA_5110_H
#define NOKIA_5110_H

#include "Arduino.h"
#include "Display_Mode.h"
#include "Mux_Rate.h"
#include "Cursor.h"

class Nokia_5110 : public Print {

    public:
        Nokia_5110();
        Nokia_5110(int8_t RST, int8_t CE, int8_t DC, int8_t DIN, int8_t CLK);
		Nokia_5110(int8_t DC, int8_t CS, int8_t RST);
		
        void setContrast(unsigned short value);
        void setTemperatureCoefficient(unsigned short value);
        void clear();
        void clear(position inRow, position fromColumn, position toColumn);
        void setCursor(position x, position y);
        void setDisplayMode(display_mode mode);
        void setBiasSystem(mux_rate rate);
        virtual size_t write(uint8_t);
		
		void Pixel(uint8_t x, uint8_t y);
		void X_line(uint8_t pos_X, uint8_t pos_Y, uint8_t _length);
		void OscilloScope(uint8_t dataport,uint8_t size_X,uint8_t size_Y,uint8_t pos_X, uint8_t pos_Y, uint16_t metric_time, uint8_t MAX_size_bit_signal);
        void clearColumn(uint8_t start_x,uint8_t end_x, uint8_t start_y,uint8_t end_y);
		void print_symbol(uint8_t symbol_cod_number);
		
		void begin();
		
		
    private:
        void _print(uint8_t charAsciiCode);
        void startTransmission();
        void endTransmission();
        void transmitInformation(byte information);
        void execute(byte command);
        void extendedInstruction();
        void basicInstruction();
        void reset();
        void initializeForSendingCommand();
        void initializeForSendingData();
        void makeEnoughSpaceForPrinting(unsigned short int newCharacterLength);
		
		void PixelBorder(uint8_t x, uint8_t y);
		
		bool ifHardwareSPI();

        int8_t _RST, _CE, _DC, _DIN, _CLK;
		uint8_t x_start = 0;

        Cursor _cursor;
};

#endif

