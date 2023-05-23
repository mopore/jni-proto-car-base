#include "jni_car_sensors_reader.h"

JniCarSensorsReader JniCarSensorsReader::s_instance;


JniCarSensorsReader& JniCarSensorsReader::getInstance(){
	return JniCarSensorsReader::s_instance;
}


JniCarSensorsReader::JniCarSensorsReader(): m_distance(), m_accelerometer() {
}


void JniCarSensorsReader::setup() {
	m_distance.setup();
	m_accelerometer.setup();
}


void JniCarSensorsReader::loop10Hz() {
	carSensors.frontDistance = m_distance.readDistance();
	auto response = m_accelerometer.readAcceleration();
	carSensors.accelX = response.accelX;
	carSensors.accelY = response.accelY;
	carSensors.accelZ = response.accelZ;
	carSensors.gyroX = response.gyroX;
	carSensors.gyroY = response.gyroY;
	carSensors.gyroZ = response.gyroZ;
	carSensors.temperatureCelsius = response.temperatureCelsius;
}
