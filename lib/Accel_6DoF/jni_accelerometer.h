#pragma once

#include <Arduino.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


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

} AccelRespone;


class JniAccelerometer{
	public:
	 	JniAccelerometer();
		void setup();
		AccelRespone readAcceleration();
	private:
		Adafruit_ICM20649 m_icm;
};
