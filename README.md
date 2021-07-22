# oscar-sensor

This project is a dog harness with sensors to detect seizures in dogs. The repo contains all the code and assets needed to build the complete prototype. Please contact me if you want a completed harness. 

The harness is an off the shelf dog harness modified with a few sensors that can determine when a dog is having a seizure and notify me on a companion app.

The project contains the following components:
* Project to work with the MPU-6050 sensor and a ESP32 microcontroller
* Project to work with the Ne-6M GPS sensor and a ESP32 microcontroller
* Project to work with the pulse sensor and a ESP32 microcontroller
* Project to connect a ESP32 microcontroller to wifi and the send data to the cloud
* Completed project with schematics to connect all the sensors to the ESP32 and send data to the cloud
* iOS app that gets a push notification when a seizure is detected

## Getting Started
If you want to experiment with each sensor, use the sample projects and connect the sensor to the ESP32 as described in each of the sample experiments. For the completed project connect all the sensors to the harness like in the image.
![Sensor location](imgs/oscar_sensor.png)

## Harness
I bought most of the components needed from Amazon. For programming the components I chose to use PlatformIO as I am trying to experiment with more poewerful IDEs than the Arduino IDE.
### Components

* [PCB Prototype board](https://amzn.to/3p4UOvW)
* [ESP32 Microcontroller](https://amzn.to/3fyBxQs)
* [Neo-6M GPS](https://amzn.to/2SJ8L6M)
* [MPU-6050 Accelerometer Gyroscope](https://amzn.to/3ySikR8)
* [Pulse sensor](https://amzn.to/3vAY70s)
* [Connecting wires](https://amzn.to/3uA37kB)
* [Dog harness](https://amzn.to/3uBMfd5)
* 9V battery power supply
* [Buck Converter](https://amzn.to/3gcxdWj)

### Connections

![Connections](imgs/connections.png)


## Azure components

* [IoT Hub (free tier)](https://azure.microsoft.com/en-us/services/iot-hub/)
* [Azure Functions (free tier)](https://azure.microsoft.com/en-us/services/functions/)
* [Notification Hub](https://azure.microsoft.com/en-us/services/notification-hubs/)

## iOS App
Build the app in Xcode.

## Future plans

* Extending the app to enable geofencing with a home controller and a Lora module.
* Collect data from multiple sensors to look for patterns in predicting seizures

## Resources

Here are some resources that I found very helpful!

* [ESP32 introduction on Dronebot Workshop](https://dronebotworkshop.com/esp32-intro/)
* [Introduction to PlatformIO on Dronebot Workshop](https://dronebotworkshop.com/platformio/)
[Working with the MPU 6050](https://randomnerdtutorials.com/esp32-mpu-6050-web-server/)
[Woking with the GPS module](https://blog.asksensors.com/iot-cloud-based-gps-tracking-esp32-gps-neo-6m-module/)
* [Connecting the ESP32 to the cloud](https://thingpulse.com/how-to-connect-your-esp32-to-the-azure-iot-cloud-over-rest/
)