#include "jni_distance.h"


JniDistance::JniDistance(){
}


void JniDistance::setup(){
	Serial.println(F("Adafruit VL53L1X sensor demo"));

	Wire.begin();
	if (! m_vl53.begin(0x29, &Wire)) {
		Serial.print(F("Error on init of VL sensor: "));
		Serial.println(m_vl53.vl_status);
		while (1)       delay(10);
	}
	Serial.println(F("VL53L1X sensor OK!"));

	if (! m_vl53.startRanging()) {
		Serial.print(F("Couldn't start ranging: "));
		Serial.println(m_vl53.vl_status);
		while (1)       delay(10);
	}
	Serial.println(F("Ranging started"));

	// Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
	m_vl53.setTimingBudget(50);
}


int16_t JniDistance::readDistance(){
	int16_t distance = -1;
	if (m_vl53.dataReady()) {
		// new measurement for the taking!
		distance = m_vl53.distance();
		if (distance == -1) {
			// something went wrong!
			Serial.print(F("Couldn't get distance: "));
			Serial.println(m_vl53.vl_status);
			return distance;
		}
		// data is read out, time for another reading!
		m_vl53.clearInterrupt();
	}
	return distance;
}