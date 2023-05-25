#pragma once

#include <Arduino.h>
#include <pwmWrite.h>  // Include dlloydev/ESP32 ESP32S2 AnalogWrite@^4.3.4
#include "jni_shared_types.h"


class JniPhysicalTranslation {
	public:
		JniPhysicalTranslation(int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed);
		int16_t translate(int16_t val_100_based) const;
	private:
		int16_t m_phys_min;
		int16_t m_phys_stop;
		int16_t m_phys_max;
		bool m_reversed;
};


class JniDriveControl {
	public:
		JniDriveControl(uint8_t pin, int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed);
		void controlDrive(int16_t val_100_based, Pwm& pwm) const;
	private:
		uint8_t m_pin;
		JniPhysicalTranslation m_translation;
};


class JniSteeringControl {
	public:
		JniSteeringControl(uint8_t pin, int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed);
		void controlSteering(int16_t val_100_based, Pwm& pwm);
	private:
		uint8_t m_pin;
		JniPhysicalTranslation m_translation;
		int16_t _lastValue = 0;
};


class JniEngineControl {
	public:
		static JniEngineControl& getInstance();
		void loop10Hz();
	private:
		JniEngineControl();
		Pwm m_pwm = Pwm();
		JniDriveControl m_leftDrive;
		JniDriveControl m_rightDrive;
		JniSteeringControl m_steering;
		static JniEngineControl s_instance;

};