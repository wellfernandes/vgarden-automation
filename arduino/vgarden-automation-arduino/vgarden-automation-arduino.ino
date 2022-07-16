#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int pinSensor = A0;  //pin used by the humidity sensor
int readedValue;           //soil moisture percentage

// Initialize the display at the address 0x3F
// the other parameters are necessary for the module to connect with the LCD
// parameter: POSITIVE Backlight ON and NEGATIVE Backlight OFF

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);

void setup() {
  Serial.begin(9600);                              //serial init
  Serial.println("Reading the soil moisture...");  //print the text on monitor serial
  delay(2000);                                     //2 seconds interval
}

void loop() {
  
}