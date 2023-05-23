JNI Proto Car Base
==================
This is the firmware project to control the base microcontroller of the JNI Proto Car.
This project rewrites the original CircuitPython code to C++ and uses the Arduino framework.

A project for to control JNI Proto Car with a PS3 controller can be found 
[here](https://github.com/mopore/jni-proto-car-control).


# Pre-requisites
* Fully setup platformIO environment
* An ESP32-S3 Microcontroller
* A Wifi network
* An MQTT broker (e.g. mosquitto)


# Setup
## Create a include/jni_config.h file
For Wifi and further network setup. Create a file called `include/jni_config.h` with the following content:

```c
#define SHOW_ENGINE_CONTROL_DEBUG 0

#define WIFI_SSID      "your-wifi-ssid"
#define WIFI_PASS      "your-wifi-password"

#define UDP_RECEIVER_SOCKET_PORT 8080

#define JNI_MQTT_BROKER_IP "192.168.100.219"
#define JNI_MQTT_BROKER_PORT 1883
```