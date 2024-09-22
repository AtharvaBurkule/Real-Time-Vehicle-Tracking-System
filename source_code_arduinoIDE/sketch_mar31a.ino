#include <TinyGPSPlus.h>
TinyGPSPlus gps;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "abc"
#define WIFI_PASSWORD "123456789"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBqWIgH1GBCccbXS8lOp-OYUg0b9SQr9HA"
// Insert RTDB URLefine the RTDB URL */
//https://vehiclecrashgps-default-rtdb.firebaseio.com/
#define DATABASE_URL "gpsaccidentdet-default-rtdb.firebaseio.com"
//https://gpsaccidentdet-default-rtdb.firebaseio.com/
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
//sketch created by Akshay Joseph



float lat, lon;
int sendsms=0;
int vibrationsen = 13;
double roll, pitch, yaw;

const int x_out = A0; /* connect x_out of module to A1 of UNO board */
const int y_out = A3; /* connect y_out of module to A2 of UNO board */
const int z_out = A6; /* connect z_out of module to A3 of UNO board */

void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduin
  pinMode(vibrationsen, INPUT);
  lcd.init();               // initialize the lcd
  lcd.backlight();          // open the backlight
  lcd.setCursor(0, 0);
  lcd.print("Real-Time Vehicle");
  lcd.setCursor(0, 1);
  lcd.print("Tracking System ");


WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
/* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  delay(2000);
  lcd.clear();
}

void loop() {


//gps
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }



int x_adc_value, y_adc_value, z_adc_value; 
  double x_g_value, y_g_value, z_g_value;

  x_adc_value = analogRead(x_out); /* Digital value of voltage on x_out pin */ 
  y_adc_value = analogRead(y_out); /* Digital value of voltage on y_out pin */ 
  z_adc_value = analogRead(z_out); /* Digital value of voltage on z_out pin */ 
  Serial.print("x = ");
  Serial.print(x_adc_value);
  Serial.print("\t\t");
  Serial.print("y = ");
  Serial.print(y_adc_value);
  Serial.print("\t\t");
if(y_adc_value>2500 || y_adc_value<1400 || x_adc_value>2500 || x_adc_value<1400  )
{
//Serial.println("Person fall detcetd");
lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fall Detected");
sendsms=1;
}
else
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No Fall Detected");

}

 int vibrationsenv = digitalRead(vibrationsen);


  if (vibrationsenv==1)
  {
  lcd.setCursor(0, 1);
  lcd.print("Vibrn:Detected");
  sendsms=1;
  delay(2000);
      lcd.clear();
  }
  else{
  lcd.setCursor(0, 1);
  lcd.print("Vibrn:Not Detected");
  delay(2000);
  lcd.clear();
  }

///////////////////////////////////
if(sendsms==1)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending SMS....");
   SendMessage();
   sendsms=0;
}

delay(100);

}


void SendMessage()
{
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+919921497349\"\r"); // Replace x with mobile number
  delay(1000);
  //mySerial.println("Bridge Break Notification");// The SMS text you want to send
  Serial.println("Accident Detected ");// The SMS text you want to send

  Serial.println("Latitude");// The SMS text you want to send

  Serial.println(lat);// The SMS text you want to send

  Serial.println("Longitude");// The SMS text you want to send

  Serial.println(lon);// The SMS text you want to send

  delay(200);
   Serial.println((char)26);// ASCII code of CTRL+Z
}


//////gps


void displayInfo()
{
  //Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    //Serial.print("Lat: ");
    lat=gps.location.lat();
    //Serial.print(gps.location.lat(), 6);
      lcd.setCursor(0, 0);
  lcd.print("Lati= ");    
  lcd.print(gps.location.lat(), 6);
   //if (Firebase.RTDB.setFloat(&fbdo
      if (Firebase.RTDB.setFloat(&fbdo, "Latitude8m", lat)){
    }
    //Serial.print(F(","));
    //Serial.print("Lng: ");
    lcd.setCursor(0, 1);
      lcd.print("Long= ");    
  lcd.print(gps.location.lng(), 6);
    lon=gps.location.lng();
    if (Firebase.RTDB.setFloat(&fbdo, "Longitude8m", lon)){
    }
    //Serial.print(gps.location.lng(), 6);
   // Serial.println();
   delay(10);
  }  

  else
  {
  lcd.setCursor(0, 0);
  lcd.print("No GPS Signal"); 
  delay(20);
  }
}

void updateSerial()
{
//   delay(500);
//   // while (Serial.available())
//   // {
//   //   Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
//   // }
//   while (Serial2.available())
//   {
//     Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
//   }
 }