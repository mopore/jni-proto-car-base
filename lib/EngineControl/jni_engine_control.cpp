#include "jni_engine_control.h"

// Drive assistance features
#define FRONT_COLLISION_DETECTION 1
#define STEERING_WITHOUT_DRIVE 1
#define STEERING_FRICTION 1


// Driving assistance values
#define SECURE_DISTANCE 200
#define MIN_DRIVE_INPUT 5
#define MAX_STEERING_PER_LOOP 10


// Engine calibrations
#define THURST_MIN 0
#define THURST_MAX 180

#define LEFT_FRONT_PIN 14
#define LEFT_STOP 94
#define LEFT_REVERSE false

#define RIGHT_FRONT_PIN 12
#define RIGHT_STOP 90
#define RIGHT_REVERSE true

#define STEERING_PIN 6
#define STEERING_CENTER 97  // The higher the value the more to the right
#define STEERING_MIN (STEERING_CENTER - 25)
#define STEERING_MAX (STEERING_CENTER + 25)
#define STEERING_REVERESE true

JniEngineControl JniEngineControl::s_instance;


JniEngineControl& JniEngineControl::getInstance(){
	return JniEngineControl::s_instance;
}


JniEngineControl::JniEngineControl() : 
	m_pwm(Pwm()),
	m_leftDrive(LEFT_FRONT_PIN, THURST_MIN, LEFT_STOP, THURST_MAX, LEFT_REVERSE),
	m_rightDrive(RIGHT_FRONT_PIN, THURST_MIN, RIGHT_STOP, THURST_MAX, RIGHT_REVERSE),
	m_steering(STEERING_PIN, STEERING_MIN, STEERING_CENTER, STEERING_MAX, STEERING_REVERESE) {
}


void JniEngineControl::loop10Hz() {
	auto driveInput = carInput.y;
	auto steeringInput = carInput.x;

	// Front collision detection	
	#if FRONT_COLLISION_DETECTION == 1
	bool collisionDanger = carInput.y > MIN_DRIVE_INPUT && 
		carSensors.frontDistance < SECURE_DISTANCE;
	if (collisionDanger) {
		driveInput = 0;
	}
	#endif
	
	// Avoid steering without drive
	#if STEERING_WITHOUT_DRIVE == 1
	if (abs(driveInput) < MIN_DRIVE_INPUT) {
		steeringInput = 0;
	}
	#endif

	m_leftDrive.controlDrive(driveInput, m_pwm);
	m_rightDrive.controlDrive(driveInput, m_pwm);
	m_steering.controlSteering(steeringInput, m_pwm);
}


static int16_t mapValue(int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max, int16_t value_to_map) {
    return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


JniPhysicalTranslation::JniPhysicalTranslation(int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed) : 
			m_phys_min(phys_min),
			m_phys_stop(phys_stop),
			m_phys_max(phys_max),
			m_reversed(reversed) {
}


int16_t JniPhysicalTranslation::translate(int16_t val_100_based) const {
	int16_t phys_val = m_phys_stop;
	if (val_100_based >= 0){
		if (m_reversed) {
			phys_val = mapValue(0, 100, m_phys_stop, m_phys_min, val_100_based);
		} else {
			phys_val = mapValue(0, 100, m_phys_stop, m_phys_max, val_100_based);
		}
	} else {
		if (m_reversed) {
			phys_val = mapValue(-100, 0, m_phys_max, m_phys_stop, val_100_based);
		} else {
			phys_val = mapValue(-100, 0, m_phys_min, m_phys_stop, val_100_based);
		}
	}
	return phys_val;
}


JniDriveControl::JniDriveControl(uint8_t pin, int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed) :
	m_pin(pin),
	m_translation(phys_min, phys_stop, phys_max, reversed){
}


void JniDriveControl::controlDrive(int16_t val_100_based, Pwm& pwm) const {
	int16_t phys_value = m_translation.translate(val_100_based);
	pwm.writeServo(m_pin, phys_value);
}


JniSteeringControl::JniSteeringControl(uint8_t pin, int16_t phys_min, int16_t phys_stop, int16_t phys_max, bool reversed) :
	m_pin(pin),
	m_translation(phys_min, phys_stop, phys_max, reversed){
}


void JniSteeringControl::controlSteering(int16_t new_val_100_based, Pwm& pwm) {

	// Use steering friction
	#if STEERING_FRICTION == 1	
	int16_t diffWanted = new_val_100_based - _lastValue;
	bool steerToRightWanted = diffWanted > 0;
	bool steerToLeftWanted = diffWanted < 0;
	bool inputNeedsReduction = abs(diffWanted) > MAX_STEERING_PER_LOOP;

	if (inputNeedsReduction) {
		if (steerToRightWanted) {
			new_val_100_based = _lastValue + MAX_STEERING_PER_LOOP;
			if (new_val_100_based > 100) {
				new_val_100_based = 100;
			}
		}  
		if (steerToLeftWanted) { // Steer to left wanted
			new_val_100_based = _lastValue - MAX_STEERING_PER_LOOP;
			if (new_val_100_based < -100) {
				new_val_100_based = -100;
			}
		}
	}
	#endif

	// Physical adaption
	int16_t phys_value = m_translation.translate(new_val_100_based);
	pwm.writeServo(m_pin, phys_value);

	_lastValue = new_val_100_based;
}