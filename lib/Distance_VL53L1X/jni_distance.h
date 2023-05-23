#pragma once

#include <Arduino.h>
#include <Adafruit_VL53L1X.h>

#define VL53L1X_IRQ_PIN 2
#define VL53L1X_XSHUT_PIN 3


class JniDistance{
	public:
	 	JniDistance();
		void setup();
		int16_t readDistance();
	private:
		Adafruit_VL53L1X m_vl53 = Adafruit_VL53L1X(VL53L1X_XSHUT_PIN, VL53L1X_IRQ_PIN);
};
