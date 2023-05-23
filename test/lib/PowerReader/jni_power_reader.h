#pragma once

#include <Arduino.h>
#include <UMS3.h>
#include "jni_shared_types.h"


class PowerReader {
	public:
		PowerReader();
		void setup();
		void updatePowerStatus();
	
	private:
		float getBatteryVoltage(); 
		esp_adc_cal_characteristics_t adc_cal;
};