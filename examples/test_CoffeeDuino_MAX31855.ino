

	 /*************************************************** 
	  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

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

	//#include "Adafruit_MAX31855.h"
	#include "CoffeeDuino_MAX31855.h"
	#include <LiquidCrystal_I2C.h>

	#include <avr/io.h>     // .h contenant les registres SFR
	#include <util/delay.h> // .h contenant les fonctions de délai
	#include <Wire.h>




	#define WHITE 0x7

	int thermoDO = A3;
	int thermoCS = A2;
	int thermoCLK = A1;

	long TimeNow;
	long LastTime;
	float Temp_ex;
	float Temp;
	Adafruit_MAX31855 thermocouple();

	LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
	  
	void setup() 
	{
		Serial.begin(9600);
		lcd.begin(16, 2);
		lcd.clear();
		lcd.setBacklight(WHITE);
		lcd.backlight();
		//Serial.println("MAX31855 test");
		// wait for MAX chip to stabilize
		delay(500);
	}

	void loop() {

	// basic readout test, just print the current temp
	char char_temp[8];
	Serial.print("Internal Temp = ");
	Temp = thermocouple.readInternal();
	Temp_ex = thermocouple.readCelsius();
	Serial.println(Temp);
	Serial.println(Temp_ex);
	lcd.setCursor(0,0);
	lcd.print("temp_chip");
	dtostrf(Temp, 6, 2, char_temp);
	lcd.setCursor(10,0);
	lcd.print(char_temp);
	lcd.setCursor(0,1);
	lcd.print("temp_ext");
	dtostrf(Temp_ex, 6, 2, char_temp);
	lcd.setCursor(10,1);
	lcd.print(char_temp);


	   
	  /* 
	   if (isnan(c)) {
		 Serial.println("Something wrong with thermocouple!");
	   } else {
		 //Serial.print("C = "); 
		 //Serial.println(c);
	   }
	   
	 */
	   delay(200);
	}