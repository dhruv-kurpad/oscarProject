#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const unsigned int writeInterval = 25000; // write interval (in ms)
static const int RXPin = 16, TXPin = 17; // UART pinout
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device
static const uint32_t GPSBaud = 9600;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (ss.available() > 0)
  if (gps.encode(ss.read()))
  {
    //displayInfo();
    
      Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
      Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
      Serial.print("ALT=");  Serial.println(gps.altitude.meters());
  }
  

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
  Serial.println(F("No GPS detected: check wiring."));
  while(true);
  }
}