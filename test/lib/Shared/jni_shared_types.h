#pragma once

#include <Arduino.h>
#include <string>

#define MAX_IP_LENGTH 16


typedef struct {
	int16_t x;
	int16_t y;
} CarInput;

extern volatile CarInput carInput;


typedef struct {
	bool usbPowerPresent;
	float batteryVoltage;
} PowerStatus;

extern volatile PowerStatus powerStatus;


typedef struct {
	float temperatureCelsius;

	// acceleration in m/s^2
	float accelX;
	float accelY;
	float accelZ;

	// angular velocity in rad/s
	float gyroX;
	float gyroY;
	float gyroZ;

	// front distance in mm
	int16_t frontDistance;
} CarSensors;

extern volatile CarSensors carSensors;


typedef struct {
	bool isWifiConnected;
	bool isUdpWorking;
	char ip_v4[MAX_IP_LENGTH];
} ConnectionSatus;

extern volatile ConnectionSatus connectionStatus;

void setWifiStatusIP_v4(const char* ipv4);
const char* getWifiStatusIP_v4();