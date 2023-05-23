#include "jni_accelerometer.h"


static uint16_t measurement_delay_us = 65535; // Delay between measurements for testing


JniAccelerometer::JniAccelerometer(){
}


void JniAccelerometer::setup(){
	Serial.println("Adafruit ICM20649 test!");

	// Try to initialize!
	if (!m_icm.begin_I2C()) {
		Serial.println("Failed to find ICM20649 chip");
		while (1) {
		delay(10);
		}
	}
	Serial.println("ICM20649 Found!");
	// icm.setAccelRange(ICM20649_ACCEL_RANGE_4_G);
	// Serial.print("Accelerometer range set to: ");
	// switch (m_icm.getAccelRange()) {
	// case ICM20649_ACCEL_RANGE_4_G:
	// 	Serial.println("+-4G");
	// 	break;
	// case ICM20649_ACCEL_RANGE_8_G:
	// 	Serial.println("+-8G");
	// 	break;
	// case ICM20649_ACCEL_RANGE_16_G:
	// 	Serial.println("+-16G");
	// 	break;
	// case ICM20649_ACCEL_RANGE_30_G:
	// 	Serial.println("+-30G");
	// 	break;
	// }

	// icm.setGyroRange(ICM20649_GYRO_RANGE_500_DPS);
	// Serial.print("Gyro range set to: ");
	// switch (m_icm.getGyroRange()) {
	// 	case ICM20649_GYRO_RANGE_500_DPS:
	// 		Serial.println("500 degrees/s");
	// 		break;
	// 	case ICM20649_GYRO_RANGE_1000_DPS:
	// 		Serial.println("1000 degrees/s");
	// 		break;
	// 	case ICM20649_GYRO_RANGE_2000_DPS:
	// 		Serial.println("2000 degrees/s");
	// 		break;
	// 	case ICM20649_GYRO_RANGE_4000_DPS:
	// 		Serial.println("4000 degrees/s");
	// 		break;
	// }

	//  icm.setAccelRateDivisor(4095);
	uint16_t accel_divisor = m_icm.getAccelRateDivisor();
	float accel_rate = 1125 / (1.0 + accel_divisor);

	Serial.print("Accelerometer data rate divisor set to: ");
	Serial.println(accel_divisor);
	Serial.print("Accelerometer data rate (Hz) is approximately: ");
	Serial.println(accel_rate);

	//  icm.setGyroRateDivisor(255);
	uint8_t gyro_divisor = m_icm.getGyroRateDivisor();
	float gyro_rate = 1100 / (1.0 + gyro_divisor);

	Serial.print("Gyro data rate divisor set to: ");
	Serial.println(gyro_divisor);
	Serial.print("Gyro data rate (Hz) is approximately: ");
	Serial.println(gyro_rate);
	Serial.println();
}


AccelRespone JniAccelerometer::readAcceleration(){
	AccelRespone response;

	//  /* Get a new normalized sensor event */
	sensors_event_t accel;
	sensors_event_t gyro;
	sensors_event_t temp;
	m_icm.getEvent(&accel, &gyro, &temp);

	// Serial.print("\t\tTemperature ");
	// Serial.print(temp.temperature);
	// Serial.println(" deg C");
	response.temperatureCelsius = temp.temperature;

	/* Display the results (acceleration is measured in m/s^2) */
	// Serial.print("\t\tAccel X: ");
	// Serial.print(accel.acceleration.x);
	response.accelX = accel.acceleration.x;
	// Serial.print(" \tY: ");
	// Serial.print(accel.acceleration.y);
	response.accelY = accel.acceleration.y;
	// Serial.print(" \tZ: ");
	// Serial.print(accel.acceleration.z);
	// Serial.println(" m/s^2 ");
	response.accelZ = accel.acceleration.z;

	/* Display the results (acceleration is measured in m/s^2) */
	// Serial.print("\t\tGyro X: ");
	// Serial.print(gyro.gyro.x);
	response.gyroX = gyro.gyro.x;
	// Serial.print(" \tY: ");
	// Serial.print(gyro.gyro.y);
	response.gyroY = gyro.gyro.y;
	// Serial.print(" \tZ: ");
	// Serial.print(gyro.gyro.z);
	// Serial.println(" radians/s ");
	response.gyroZ = gyro.gyro.z;
	// Serial.println();
	return response;
}