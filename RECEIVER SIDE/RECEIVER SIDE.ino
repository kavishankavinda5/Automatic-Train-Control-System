//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <TinyGPS++.h>


//-------------------------------Send GPS Data---------------------------------
//#define RXD2 16
#define RXD2 16
//#define TXD2 17
#define TXD2 17

SoftwareSerial neogps(RXD2, TXD2);
TinyGPSPlus gps;

// 1. Define the WiFi credentials
#define WIFI_SSID "KAVISHAN-PC"
#define WIFI_PASSWORD "kavishan123"

// 2. Define the API Key
#define API_KEY "AIzaSyBfbpXfndk4P5YItCXK12YokwOajVgKMg0"

// 3. Define the project ID
#define FIREBASE_PROJECT_ID "live-train-365007"

// 4. Define the user Email and password
#define USER_EMAIL "kavishankavinda5@gmail.com"
#define USER_PASSWORD "012345"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String uid;
String path;
String firebaseStatus = "on";
//-----------------------------------------------------------------------------


//-------set the LCD address to 0x27 for a 16 chars and 2 line display---------
LiquidCrystal_I2C lcd(0x27, 20, 4);
//-----------------------------------------------------------------------------

//---------------------------------Get Data------------------------------------
//create an RF24 object
RF24 myRadio(2, 4);  // CE, CSN

//address through which two modules communicate.
byte addresses[][6] = { "0" };

struct package {
  int id = 0;
  int dutyCycleValue = 0;
  char text[100] = "empty";
};

typedef struct package Package;
Package data;
//-----------------------------------------------------------------------------


//--------------------------------Motor Driver---------------------------------
// Motor A connections
int enA = 14;
int in1 = 26;
int in2 = 27;
// Motor B connections
int enB = 25;
int in3 = 33;
int in4 = 32;
//-----------------------------------------------------------------------------


//---------------------------RPM & Speed & Distance----------------------------
float rpm;
int halleffect_pin = 12;
unsigned long millisBefore;
volatile int turn;
int totalTurns = 0;

float kmh;
const int wheelDiameter = 7;  // 7 cm
float distance = 0;
//-----------------------------------------------------------------------------


//---------------------------------Functions-----------------------------------
void gps_loop();
//-----------------------------------------------------------------------------

void setup() {

  Serial.begin(115200);

  //-----------------------------------LCD Display------------------------------
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
  //-----------------------------------------------------------------------------


  //---------------------------------Get Data------------------------------------
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  //-----------------------------------------------------------------------------


  //-------------------------------Send GPS Data---------------------------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.setCursor(0, 0);  // set cursor to first column, first row
  lcd.print("   Connecting To   ");
  lcd.setCursor(5, 1);  // set cursor to fifth column, second row
  lcd.print("Wi-Fi");

  int i = 10;
  while (WiFi.status() != WL_CONNECTED) {
    if (i < 13) {
      Serial.print(".");
      lcd.setCursor(i, 1);  // set cursor
      lcd.print(".");
      delay(300);
      i++;
    } else {
      i = 10;
      lcd.clear();
      Serial.print("Connecting to Wi-Fi");
      lcd.setCursor(0, 0);  // set cursor to first column, first row
      lcd.print("   Connecting To   ");
      lcd.setCursor(6, 1);  // set cursor to fifth column, second row
      lcd.print("Wi-Fi");
    }
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  lcd.setCursor(4, 2);  // set cursor to fifth column, third row
  lcd.print("Connected");
  delay(1000);
  lcd.clear();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  //Begin serial communication with neogps
  neogps.begin(9600);
  Serial.println("neogps serial initialize");
  //-----------------------------------------------------------------------------


  //--------------------------------Motor Driver---------------------------------
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  //-----------------------------------------------------------------------------


  //-----------------------------------RPM---------------------------------------
  pinMode(halleffect_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(halleffect_pin), count, FALLING);
  //-----------------------------------------------------------------------------
}

void loop() {

  unsigned long currentTime = millis();

  getData();
  motorDriver();
  speed_distance_print_to_LCD();
  gps_loop();
}

//---------------------------------Get Data-------------------------------------
void getData() {
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read(&data, sizeof(data));
    }
    Serial.print("\nPackage:");
    Serial.println(data.id);
    Serial.println(data.dutyCycleValue);
    Serial.println(data.text);
  }
}
//-----------------------------------------------------------------------------


//---------------------------------Motor Driver---------------------------------
void motorDriver() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  if (data.dutyCycleValue >= 50 && data.dutyCycleValue <= 200) {
    analogWrite(enA, data.dutyCycleValue);
    analogWrite(enB, data.dutyCycleValue);
    Serial.print("dutyCycle  = ");
    Serial.println(data.dutyCycleValue);
  }
}
//-----------------------------------------------------------------------------


//---------------------------RPM & Speed & Distance----------------------------

void speed_distance_print_to_LCD() {
  lcd.setCursor(0, 0);
  lcd.print("SPEED     : ");
  lcd.print(kmh,0);
  lcd.print(" km/h");
  lcd.setCursor(0, 1);
  lcd.print("DISTANCE  : ");
  lcd.print(distance);
  lcd.print(" km");
  lcd.setCursor(0, 2);
  lcd.print("PACKAGE ID: ");
  lcd.print(data.id);
  lcd.print("   ");

  Serial.print("RPM   : ");
  Serial.print(rpm);
  Serial.println("    ");

  if (millis() - millisBefore > 1000) {
    rpm = turn * 60.0;
    turn = 0;
    millisBefore = millis();
  }

  kmh = wheelDiameter * rpm * 0.001885;
  distance = 220 * totalTurns * 0.000001;
}

void count() {
  turn++;
  totalTurns++;
}
//-----------------------------------------------------------------------------


//-------------------------------Send GPS Data---------------------------------
void gps_loop() {

  while (neogps.available() > 0) {
    gps.encode(neogps.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);

      double latitude, longitude;

      latitude = gps.location.lat();   // Latitude in degrees (double)
      longitude = gps.location.lng();  // Longitude in degrees (double)

      if (WiFi.status() == WL_CONNECTED && Firebase.ready()) {
        String documentPath = "location/user1";

        FirebaseJson content;

        //content.set("fields/latitude/doubleValue", String(latitude).c_str());
        content.set("fields/latitude/doubleValue", latitude);
        content.set("fields/longitude/doubleValue",longitude);
        content.set("fields/name/stringValue", "MUTHUKUMARI EXPRESS");

        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "latitude,longitude,name")) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          return;
        } else {
          Serial.println(fbdo.errorReason());
        }

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          return;
        } else {
          Serial.println(fbdo.errorReason());
        }
      }
    }
  }
}
//-----------------------------------------------------------------------------
