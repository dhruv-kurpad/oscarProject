#include <Arduino.h>
#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include <Arduino_JSON.h>

// Please input the SSID and password of WiFi
const char* ssid     = "Your Wifi SSID";
const char* password = "Your Wifi Password";

/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=foobar.azure-devices.net;DeviceId=oscar;SharedAccessKey=SomeKey";

static bool hasIoTHub = false;





void setup() {
  Serial.begin(115200);
  Serial.println("Starting connecting WiFi.");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  
  Serial.println("Setup Complete \n");
}

void loop() {
  Serial.println("start sending events.");
  if (hasIoTHub)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
    snprintf(buff, 128, "{\"orientation\":\"sleeping\", \"x\" : %d, \"y\": 555}", rand());
    
   if (Esp32MQTTClient_SendEvent(buff))
    {
      Serial.println("Sending data succeed");
    }
    else
    {
      Serial.println("Failure...");
    } 
    delay(5000);

  }


}