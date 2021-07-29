#include <Arduino.h>

#define TOUCH_THRESHOLD 40
#define LED 13
void callback(){}

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("ESP32 has started");
  pinMode(LED, OUTPUT);
  
  delay(1000);
  digitalWrite(LED, HIGH);
  touchAttachInterrupt(T0, callback, TOUCH_THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  delay(1000);
  digitalWrite(LED, LOW);
  esp_deep_sleep_start();

}

void loop() {
}