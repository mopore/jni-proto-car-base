#pragma once

#include <Arduino.h>
#include "jni_shared_types.h"
#include "jni_distance.h"
#include "jni_accelerometer.h"


class JniCarSensorsReader {
	public:
		static JniCarSensorsReader& getInstance();
		void setup();
		void loop10Hz();
	private:
		JniCarSensorsReader();
		JniDistance m_distance;
		JniAccelerometer m_accelerometer;
		static JniCarSensorsReader s_instance;
};