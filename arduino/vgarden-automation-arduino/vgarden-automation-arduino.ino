#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pin used by the humidity sensor
const int pinSensor = A0;

// pin used by the relay
const int pinRelay = 7;

// moisture percentage
const int drySoil = 60;
const int dampSoil = 80;
const int soakedSoil = 95;

// irrigation state on = 1 or off = 0
bool irrigationState = 0;

int readValue;  //soil moisture percentage

// Initialize the display at the address 0x3F
// the other parameters are necessary for the module to connect with the LCD
// parameter: POSITIVE Backlight ON and NEGATIVE Backlight OFF

LiquidCrystal_I2C displayLcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);

void setup() {
  Serial.begin(9600);
  Serial.println("Vgarden...");  // Reading the soil moisture

  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, LOW);


  // initialize the display with 16 columns x 2 rows or 20,4 if it's the display 20x4
  displayLcd.begin(16, 2);
  displayLcd.setBacklightPin(3, POSITIVE);  // NEGATIVE is the black letter and dark background

  // set the backlight to on
  displayLcd.setBacklight(HIGH);
  displayLcd.clear();
}

void loop() {
  Wire.begin();
  readValue = analogRead(pinSensor);

  Serial.println(readValue);

  readValue = map(readValue, 290, 1022, 100, 0);  // sensor - soil value, maximum, divisible and positive
  Serial.print("Umidade do solo: ");
  Serial.print(readValue);
  Serial.println("%");

  displayLcd.home();
  displayLcd.setCursor(0, 0);
  displayLcd.print("Humidade:    ");
  displayLcd.setCursor(10, 0);
  displayLcd.print(readValue);
  displayLcd.setCursor(14, 0);
  displayLcd.print("% ");

  if (readValue <= drySoil) {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo muito seco   ");  // very dry soil
    if (irrigationState == 0) {
      irrigationState = 1;
      digitalWrite(pinRelay, HIGH);
      delay(10000);
      digitalWrite(pinRelay, LOW);
    }
    digitalWrite(pinRelay, HIGH);
  } else if (readValue > drySoil && readValue < dampSoil) {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo pouco seco   ");  // dry soil
  } else if (readValue >= dampSoil && readValue < soakedSoil) {
    if (irrigationState == 1) {
      irrigationState = 0;
      digitalWrite(pinRelay, LOW);
    }
    displayLcd.setCursor(0, 1);
    displayLcd.print("Umidade ideal  ");  // ideal soil
  } else if (readValue >= soakedSoil) {
    irrigationState = 1;
    displayLcd.setCursor(0, 1);
    displayLcd.print("Solo encharcado   ");  // soggy soil
  } else {
    displayLcd.setCursor(0, 1);
    displayLcd.print("Erro na leitura   ");  // sensor reading error
  }
  delay(500);  // 3 seconds interval
}