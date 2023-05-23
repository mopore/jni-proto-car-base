#include <Arduino.h>
#include "jni_shared_types.h"
#include "jni_controller_display.h"
#include "jni_input_reader.h"
#include "jni_power_reader.h"
#include "jni_wifi.h"
#include "jni_udp_receiver.h"
#include "jni_config.h"
#include "jni_engine_control.h"
#include "jni_car_sensors_reader.h"
#include "jni_mqtt_broker.h"


#define FREQ_100HZ 10  // Every 10ms of 1000ms
#define FREQ_10HZ 100  // Every 100ms of 1000ms
#define FREQ_1HZ 1000  // Every second


void readInputTask(void* pvParameters) {
	try{
		InputReader inputReader(UDP_RECEIVER_SOCKET_PORT);
		inputReader.setup();

		const TickType_t xFrequency = pdMS_TO_TICKS(FREQ_100HZ);
		while (true) {
			inputReader.loop100Hz();
			vTaskDelay(xFrequency);
		}
	}
	catch(const std::exception& e) {
		Serial.print("Caught exception in readInputTask: ");
		Serial.println(e.what());
	}
}


void displayTask(void* pvParameters) {
	try{
		ControllerDisplay controllerDisplay;
		controllerDisplay.setup();

		const TickType_t xFrequency = pdMS_TO_TICKS(FREQ_10HZ);
		while(true) {
			controllerDisplay.loop10Hz();
			vTaskDelay(xFrequency);
		}
	}
	catch(const std::exception& e) {
		Serial.print("Caught exception in displayTask: ");
		Serial.println(e.what());
	}
}


void controlEngineTask(void* pvParameters) {
	try{
		JniEngineControl& engineControl = JniEngineControl::getInstance();
		
		const TickType_t xFrequency = pdMS_TO_TICKS(FREQ_10HZ);
		auto previousWakeTime = xTaskGetTickCount();

		while(true) {
			engineControl.loop10Hz();
			auto elapsedTime = xTaskGetTickCount() - previousWakeTime;

			#if SHOW_ENGINE_CONTROL_DEBUG == 1
			// Print the elapsed time on the serial monitor
			Serial.print("Elapsed Time (ms): ");
			Serial.println(elapsedTime * portTICK_PERIOD_MS);
			previousWakeTime = xTaskGetTickCount();
			#endif

			vTaskDelay(xFrequency);
		}
	}
	catch(const std::exception& e) {
		Serial.print("Caught exception in controlEngineTask: ");
		Serial.println(e.what());
	}
}


void readSensorsTask(void* pvParameters) {
	try{
		JniCarSensorsReader& reader = JniCarSensorsReader::getInstance();
		reader.setup();

		PowerReader powerReader;
		powerReader.setup();
		uint8_t loopCounter = 0;

		const TickType_t xFrequency = pdMS_TO_TICKS(FREQ_10HZ);
		while(true) {
			reader.loop10Hz();
			loopCounter++;
			if (loopCounter == 30){
				powerReader.updatePowerStatus();
				loopCounter = 0;
			}
			vTaskDelay(xFrequency);
		}
	}
	catch(const std::exception& e) {
		Serial.print("Caught exception in readSensorsTask: ");
		Serial.println(e.what());
	}
}


void updateMqttTask(void* pvParameters) {
	try{
		JniMqttBroker& mqttBroker = JniMqttBroker::getInstance();
		mqttBroker.setup(JNI_MQTT_BROKER_IP, JNI_MQTT_BROKER_PORT);

		const TickType_t xFrequency = pdMS_TO_TICKS(FREQ_1HZ);
		while (true) {
			mqttBroker.loop1Hz();
			vTaskDelay(xFrequency);
		}
	}
	catch(const std::exception& e) {
		Serial.print("Caught exception in MQTT Task: ");
		Serial.println(e.what());
	}
}


void setup() {
	Serial.begin(115200);
	connectionStatus.isWifiConnected = false;
	connectionStatus.isUdpWorking = false;
	setWifiStatusIP_v4(NO_IP);
	char out_ip[MAX_IP_LENGTH];
	connect_wifi(out_ip, WIFI_SSID, WIFI_PASS);
	if (strcmp(out_ip, NO_IP) != 0) {
		Serial.printf("Connected with IP address: %s\n", out_ip);
		setWifiStatusIP_v4(out_ip);
		connectionStatus.isWifiConnected = true;
	
		xTaskCreate(displayTask, "displayTask", 4096, NULL, 1, NULL);
		xTaskCreate(readInputTask, "readInputTask", 4096, NULL, 1, NULL);	
		xTaskCreate(readSensorsTask, "readSensorsTaks", 4096, NULL, 1, NULL);	
		xTaskCreate(controlEngineTask, "controlEngineTask", 4096, NULL, 1, NULL);
		xTaskCreate(updateMqttTask, "updateMqttTask", 4096, NULL, 1, NULL);
	}
}


void loop() {
	// We don't need to do anything here.
}