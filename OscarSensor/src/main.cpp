#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include "driver/periph_ctrl.h"

const char* ssid     = "Your Wifi SSID";
const char* password = "Your Wifi Password";
const char* server = "Azure function URL"
  float roll;
  unsigned long count;
} Message;
sensors_event_t a, g, temp;
unsigned long lastMillis = 0;
struct Message messages[100];
int arr_count = 0;
bool connectToWifi()
{
    WiFi.begin(ssid, password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count == 10)
        return false;
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    return true;
}
void initMPU()
{
  // Try to initialize!
  if (!mpu.begin()) {

    while (1) {
      Serial.println("Failed to find MPU6050 chip");
      delay(10);
      if(mpu.begin())
        break;
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("MPU6050 Found!");
}

void publishReadings( void * parameter) {
  for(;;) {
    //Serial.println(buff);
    if (xQueueReceive(msg_queue, (void *)&rcv_msg, 0) == pdTRUE) {
      Serial.println("SENDING HTTP REQUEST");
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        http.begin(server);
        http.addHeader("Content-Type", "text/plain");       
        // Send HTTP POST request
        Serial.println(rcv_msg);
        int httpResponseCode = http.POST(rcv_msg);

        Serial.print("HTTP Response code is: ");
        Serial.println(httpResponseCode);
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectToWifi();
  initMPU();
  Serial.println(xPortGetCoreID());
  msg_queue = xQueueCreate(3, sizeof(buff));
  xTaskCreatePinnedToCore(
      publishReadings, /* Function to implement the task */
      "publishReadings", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &PublishReadings,  /* Task handle. */
      0); /* Core where the task should run */
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  if(arr_count == 0)
  {
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print("Y: ");
    Serial.print(a.acceleration.y);
    Serial.print("Z: ");
    Serial.println(a.acceleration.z);
  }
  
  
  if(arr_count < 99) {
    if(first)
    {
      Message msg;
      msg.yaw = a.acceleration.x;
      msg.pitch = a.acceleration.y;
      msg.roll = a.acceleration.z;
      messages[arr_count] = msg;
    }
    else
    {
      messages[arr_count].yaw = a.acceleration.x;
      messages[arr_count].pitch = a.acceleration.y;
      messages[arr_count].roll = a.acceleration.z;
    }
    
    arr_count++;
    vTaskDelay(100);
  }
  else
  {
    Serial.println("Array FULL!!");
    int length = sizeof(messages)/sizeof(messages[0]);   
    for(int i=0; i<100; i++)
    {
      if(i == 0) {
        sprintf(buff,"%f,%f,%f", messages[0].yaw, messages[0].pitch,messages[0].roll);
      }
        
      else{  
        sprintf(buff + strlen(buff),";%f,%f,%f", messages[i].yaw, messages[i].pitch,messages[i].roll);
      }

    }
    Serial.println("Adding message to queue");
    xQueueSend(msg_queue, (void *)&buff, 10);
    //memset(messages, 0, sizeof(messages));
    first = false;
    //delay 10 seconds
    delay(1000);
    arr_count = 0;
  }
  Serial.println("Bottom of loop");
}