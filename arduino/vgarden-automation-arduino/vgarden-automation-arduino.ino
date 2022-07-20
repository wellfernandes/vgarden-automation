#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pin used by the humidity sensor
const int pinSensor = A0;

// pin used by the relay
const int pinRelay = 7;

// irrigation indication leds
const int pinLedOn = 2;
const int pinLedOff = 3;

// moisture percentage
const int drySoil = 60;
const int dampSoil = 80;
const int soakedSoil = 95;

// irrigation state on = 1 or off = 0
bool irrigationState = 0;

//soil moisture percentage
int readValue;  

// Initialize the display at the address 0x3F
// the other parameters are necessary for the module to connect with the LCD
// parameter: POSITIVE Backlight ON and NEGATIVE Backlight OFF
LiquidCrystal_I2C displayLcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);

void setup() {
  Serial.begin(9600);

  //Leds On and Off
  pinMode(pinLedOn, OUTPUT);
  pinMode(pinLedOff, OUTPUT);
  digitalWrite(pinLedOn, LOW);
  digitalWrite(pinLedOff, HIGH);
  
   // initialize the display with 16 columns x 2 rows or 20,4 if it's the display 20x4
  displayLcd.begin(16, 2);
  displayLcd.setBacklightPin(3, POSITIVE);  // NEGATIVE is the black letter and dark background

  // set the backlight to on
  displayLcd.setBacklight(HIGH);
  displayLcd.clear();

  displayLcd.home();
  displayLcd.setCursor(0, 0);
  displayLcd.println("     Vgarden    "); 
  displayLcd.setCursor(0, 1);
  displayLcd.println("   iniciando...  "); // intro text - getting started
  delay(10000);

  displayLcd.clear();
  displayLcd.home();
  displayLcd.setCursor(0, 0);
  displayLcd.println("recebendo dados "); 
  displayLcd.setCursor(0, 1);
  displayLcd.println("   aguarde...   ");
  delay(5000);
  displayLcd.clear();
}

void loop() {
  Wire.begin();
  readValue = analogRead(pinSensor);

  Serial.println(readValue);

  // sensor - soil value, maximum, divisible and positive
  readValue = map(readValue, 290, 1022, 100, 0);  
  // Serial.print("Umidade do solo: "); // to print in the terminal
  // Serial.print(readValue);
  // Serial.println("%  ");
  
  displayLcd.home();
  displayLcd.setCursor(0, 0);
  displayLcd.print("Humidade:    ");
  displayLcd.setCursor(10, 0);
  displayLcd.print(readValue);
  displayLcd.setCursor(14, 0);
  displayLcd.print("%  ");

  if (readValue <= drySoil) {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo muito seco   ");  // very dry soil
    if (irrigationState == 0) {
      irrigationState = 1;
      digitalWrite(pinRelay, HIGH);
      digitalWrite(pinLedOn, HIGH);
      digitalWrite(pinLedOff, LOW);
    }
    digitalWrite(pinRelay, HIGH);
  } else if (readValue > drySoil && readValue < dampSoil) {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo pouco seco   ");  // dry soil
  } else if (readValue >= dampSoil && readValue < soakedSoil) {
    if (irrigationState == 1) {
      irrigationState = 0;
      digitalWrite(pinRelay, LOW);
      digitalWrite(pinLedOn, LOW);
      digitalWrite(pinLedOff, HIGH);
    }
    displayLcd.setCursor(0, 1);
    displayLcd.print("Umidade ideal  ");  // ideal soil
    digitalWrite(pinLedOn, LOW);
    digitalWrite(pinLedOff, HIGH);
  } else if (readValue >= soakedSoil) {
    irrigationState = 1;
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo encharcado   ");  // soggy soil
    digitalWrite(pinLedOn, LOW);
    digitalWrite(pinLedOff, HIGH);
  } else {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Erro na leitura   ");  // sensor reading error
    digitalWrite(pinLedOn, LOW);
    digitalWrite(pinLedOff, HIGH);
    delay(2000);
    digitalWrite(pinLedOff, HIGH);
  }
  // 2 seconds interval
  delay(2000);  
}