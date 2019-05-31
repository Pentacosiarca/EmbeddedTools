/*
Control of D/A converter MCP4921 with Arduino UNO

SPI  |  MCP4921  |  UNO
------------------------
SS/CS|  2        |  10
MOSI |  4        |  11
SCK  |  3        |  13

16 bits to transmit:

12 - 15 are control registers
0 - 11  digit to display 

Based on the example by Tom Igoe about controlling a digital potentiometer with SPI.
*/


// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select:
const byte slaveSelectPin = 10;
const uint16_t controlBits = 0b0011000000000000;

void setup() {
  Serial.begin(9600);
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
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
  digitalWrite(slaveSelectPin, LOW);
  //SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(msb);
  SPI.transfer(lsb);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
}
