/*
Control of D/A converter MCP4921 with Attiny85

SPI  |  MCP4921  |  Attiny85
------------------------
SS/CS|  2        |  0
MOSI |  4        |  1
SCK  |  3        |  2

16 bits to transmit:

12 - 15 are control registers
0 - 11  digit to display 

Based on the library and example by 
https://github.com/JChristensen/tinySPI.git
*/



#include <tinySPI.h>

const uint16_t controlBits = 0b0011000000000000;

// pin definitions
const byte
    LATCH_PIN(0),               // storage register clock (slave select)
    DATA_PIN(1),                // data in
    CLOCK_PIN(2);               // shift register clock

void setup()
{
    // pullups on for unused pins to minimize power consumption
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    
    pinMode(CLOCK_PIN, OUTPUT);    // set up the pins for software SPI
    pinMode(DATA_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);    // latch pin needs to be set up for hardware or software
    digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
  // change the output value of the output from min to max (12 bit = 4095):
  for (uint16_t AnVal = 0; AnVal < 4095; AnVal+=100) {
    // add the configuration bits to the 12 bit digital output:
    uint16_t value = controlBits + AnVal;
    byte lsb = (unsigned)value & 0xff; // mask the lower 8 bits
    byte msb = (unsigned)value >> 8;   // shift the higher 8 bits
    
    digitalAnalogOutput(msb, lsb);
    delay(10);
  }

}

void digitalAnalogOutput(int msb, int lsb) {
  // take the SS pin low to select the chip:
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, msb);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lsb);
  // take the SS pin high to de-select the chip:
  digitalWrite(LATCH_PIN, HIGH);
}
