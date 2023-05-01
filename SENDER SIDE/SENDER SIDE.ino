#include <Adafruit_NeoPixel.h>

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int kmh = 0;

//create an RF24 object
RF24 myRadio(2, 4);  // CE, CSN

//address through which two modules communicate.
byte addresses[][6] = { "0" };

struct package {
  int id = 1;
  int dutyCycleValue = 50;
  char text[100] = "Package Transmitted";
};


typedef struct package Package;
Package data;

void setup() {

  Serial.begin(115200);

  lcd.begin();      // initialize the LCD,
  lcd.backlight();  // Turn on the blacklight and print a message.
  lcd.clear();
  lcd.setCursor(0, 0);                // set cursor to first column, first row
  lcd.print("-----ATC SYSTEM-----");  // print message
  lcd.setCursor(0, 2);                //set cursor to second column, first row
  lcd.print(" Please Wait");
  int k;
  for (k = 0; k < 3; k++) {
    lcd.setCursor(k+12, 2);  // set cursor
    lcd.print(".");
    delay(300);
  }
  delay(1000);
  lcd.clear();

  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openWritingPipe(addresses[0]);
}

void loop() {

  if (data.dutyCycleValue >= 50 && data.dutyCycleValue <= 200) {

    myRadio.write(&data, sizeof(data));

    Serial.print("\nPackage:");
    Serial.println(data.id);
    Serial.println(data.dutyCycleValue);
    Serial.println(data.text);

    lcd.setCursor(0, 0);
    lcd.print("PACKAGE ID: ");
    lcd.print(data.id);
    lcd.setCursor(0, 1);
    lcd.print("SET SPEED : ");
    lcd.print(kmh,0);
    lcd.print("km/h");
    lcd.setCursor(0, 2);
    lcd.print(data.text);

    data.id += 1;

    if (data.dutyCycleValue >= 40 && data.dutyCycleValue <= 50) {
      kmh = 1;
    } else if (data.dutyCycleValue > 50 && data.dutyCycleValue <= 60) {
      kmh = 2;
    } else if (data.dutyCycleValue > 70 && data.dutyCycleValue <= 80) {
      kmh = 3;
    } else if (data.dutyCycleValue > 80 && data.dutyCycleValue <= 150) {
      kmh = 4;
    } else if (data.dutyCycleValue > 150 && data.dutyCycleValue <= 180) {
      kmh = 5;
    } else if (data.dutyCycleValue > 180 && data.dutyCycleValue <= 200) {
      kmh = 6;
    }

    data.dutyCycleValue += 10;



  } else {
    data.dutyCycleValue = 50;
  }

  delay(5000);
}
