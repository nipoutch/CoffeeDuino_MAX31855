/*************************************************** 
  This is a library for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
 /*********************************
 
 Optimisation CofeeDuino
int MISO 	= A3; =>>	PC3	=>>
int CS 		= A2; =>>	PC2 =>>
int SCLK 	= A1; =>>	PC1 =>>

pinMode(x, OUTPUT); DDRX |= (1 << DDXx)
pinMode(x, INPUT);	DDRX &= ~(1 << DDXx);

PORTX |= (1 << PINx);//digitalWrite(commande_relais, HIGH);
PORTX &= ~(1 << PINx); // equivalent digitalWrite(commande_relais, LOW); en AVRC
PINX & (1 << DDXx) //digitalRead(x)
**********************************/

#include "CoffeeDuino_MAX31855.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>


Adafruit_MAX31855::Adafruit_MAX31855() {
  //sclk = SCLK;
  //cs = CS;
  //miso = MISO;

  //define pin modes
  
  //pinMode(sclk, OUTPUT);
  DDRC |= (1 << DDC1);
  //pinMode(cs, OUTPUT);
  DDRC |= (1 << DDC2);
  //pinMode(miso, INPUT);
  DDRC &= ~(1 << DDC3);

  //digitalWrite(cs, HIGH);
  PORTC |= (1 << PIN2);
}


double Adafruit_MAX31855::readInternal(void) 
{
	uint32_t v;

  v = spiread32();

  // ignore bottom 4 bits - they're just thermocouple data
  v >>= 4;

  // pull the bottom 11 bits off
  float internal = v & 0x7FF;
  internal *= 0.0625; // LSB = 0.0625 degrees
  // check sign bit!
  if (v & 0x800) 
    internal *= -1;
  //Serial.print("\tInternal Temp: "); Serial.println(internal);
  return internal;
}

double Adafruit_MAX31855::readCelsius(void) {

  int32_t v;

  v = spiread32();

  //Serial.print("0x"); Serial.println(v, HEX);

  /*
  float internal = (v >> 4) & 0x7FF;
  internal *= 0.0625;
  if ((v >> 4) & 0x800) 
    internal *= -1;
  Serial.print("\tInternal Temp: "); Serial.println(internal);
  */

  if (v & 0x7) {
    // uh oh, a serious problem!
    return NAN; 
  }

  // get rid of internal temp data, and any fault bits
  v >>= 18;
  //Serial.println(v, HEX);
  
  double centigrade = v;

  // LSB = 0.25 degrees C
  centigrade *= 0.25;
  return centigrade;
}

uint8_t Adafruit_MAX31855::readError() {
  return spiread32() & 0x7;
}

double Adafruit_MAX31855::readFarenheit(void) {
  float f = readCelsius();
  f *= 9.0;
  f /= 5.0;
  f += 32;
  return f;
}

uint32_t Adafruit_MAX31855::spiread32(void) { 
  int i;
  uint32_t d = 0;

  //digitalWrite(sclk, LOW);
  PORTC &= ~(1 << PIN1);
  _delay_us(50);
  //digitalWrite(cs, LOW);
  PORTC &= ~(1 << PIN2);
  _delay_us(50);

  for (i=31; i>=0; i--)
  {
	//digitalWrite(sclk, LOW);
    PORTC &= ~(1 << PIN1);
    _delay_us(50);
    d <<= 1;
	//digitalRead(miso);
    if (PINC & (1 << DDC3)) {
      d |= 1;
    }

	//digitalWrite(sclk, HIGH);
    PORTC |= (1 << PIN1);
    _delay_us(50);
  }
  
	//digitalWrite(cs, HIGH);
  PORTC |= (1 << PIN2);
  //Serial.println(d, HEX);
  return d;
}
