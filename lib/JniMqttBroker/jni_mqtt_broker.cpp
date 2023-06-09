#include "jni_mqtt_broker.h"

JniMqttBroker JniMqttBroker::s_instance;


JniMqttBroker& JniMqttBroker::getInstance() {
	return JniMqttBroker::s_instance;
}


// Private constructor...
JniMqttBroker::JniMqttBroker() :
	_espClient(),
	_client(_espClient) {
}


void JniMqttBroker::setup(const char* brokerIp, const int brokerPort) {
	_client.setServer(brokerIp, brokerPort);
	_client.setCallback(JniMqttBroker::onMessage);
	delay(1500);
	_lastReconnectAttempt = 0;
}


// Static method to pass messages to instance
void JniMqttBroker::onMessage(char* topic, byte* payload, unsigned int length) {
	s_instance._handleMessage(topic, payload, length);
} 


void JniMqttBroker::_handleMessage(char* topic, byte* payload, unsigned int length){
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	
	// Convert payload to string
	std::string payload_str;
	for (int i = 0; i < length; i++) {
		payload_str += (char)payload[i];
	}

	Serial.println(payload_str.c_str());
}


boolean JniMqttBroker::_reconnect() {
	Serial.println("Attempting MQTT connection...");
	if (_client.connect(MQTT_CLIENT_NAME)) {
		Serial.println("Connected to MQTT broker");
		_aliveCounter = SEND_ALIVE_SECONDS;
		_client.publish(TOPIC_ALIVE,"hello world");
		// TODO Make something with the input subsrciption...
		_client.subscribe(TOPIC_INPUT);
	}
	else {
		Serial.print("Failed, rc=");
		Serial.print(_client.state());
		Serial.println(" try again in 5 seconds");
	}
	return _client.connected();
}


void JniMqttBroker::_checkReconnect() {
	long now = millis();
	if (now - _lastReconnectAttempt > 5000) {
		_lastReconnectAttempt = now;
		// Attempt to reconnect
		if (_reconnect()) {
			_lastReconnectAttempt = 0;
		}
	}
}


void JniMqttBroker::loop1Hz() {
	if (_client.connected()) {
		_client.loop();
		_ensureAliveTick();
		_sendSensorData();
	} else {
		_checkReconnect();
	}
}


void JniMqttBroker::_ensureAliveTick() {
	if (_aliveCounter == SEND_ALIVE_SECONDS){
		auto connectedIp = getWifiStatusIP_v4();
		_client.publish(TOPIC_ALIVE, connectedIp);
		_aliveCounter = 0;
	} else {
		_aliveCounter++;
	}
}


void JniMqttBroker::_sendSensorData() {
	char jsonOutput[440];  // Input and output size calculated with:
	// char jsonOutput[360];  // Input and output size calculated with:
	// https://arduinojson.org/v6/assistant/
	/*
	Example output:
		{
		"temperatureCelsius": 32.245678901234567890123456789,
		"accelX": -2.7182818284590452353602874713527,
		"accelY": -2.7182818284590452353602874713527,
		"accelZ": -2.7182818284590452353602874713527,
		"gyroX": -2.7182818284590452353602874713527,
		"gyroY": -2.7182818284590452353602874713527,
		"gyroZ": -2.7182818284590452353602874713527,
		"frontDistance": 7650,
		"batteryVoltageUnplugged": 3.2425678901234567890123456789
		}
	*/    
	StaticJsonDocument<192> doc;  // To be created on the stack
	// StaticJsonDocument<128> doc;  // To be created on the stack

    doc["temperatureCelsius"] = carSensors.temperatureCelsius;

    doc["accelX"] = carSensors.accelX;
    doc["accelY"] = carSensors.accelY;
	doc["accelZ"] = carSensors.accelZ;
    
	doc["gyroX"] = carSensors.gyroX;
	doc["gyroY"] = carSensors.gyroY;
	doc["gyroZ"] = carSensors.gyroZ;

	doc["frontDistance"] = carSensors.frontDistance;

	bool carIsNotCharging = !powerStatus.usbPowerPresent;
	if (carIsNotCharging) {
		doc["batVoltUnplug"] = powerStatus.batteryVoltage;
	} 
	serializeJson(doc, jsonOutput);
	_client.publish(TOPIC_SENSORS, jsonOutput);
}