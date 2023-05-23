#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>  // knolleary/PubSubClient@^2.8
#include "jni_shared_types.h"


#define MQTT_CLIENT_NAME "jniProtoCar"
#define TOPIC_SERVICE_PREFIX "jniHome/services/jniProtoCar"
#define TOPIC_ALIVE TOPIC_SERVICE_PREFIX "/alive"
#define TOPIC_INPUT TOPIC_SERVICE_PREFIX "/input"
#define SEND_ALIVE_SECONDS 5


class JniMqttBroker {
	public:
	 	static JniMqttBroker& getInstance();
		static void onMessage(char* topic, byte* payload, unsigned int length);

		void setup(const char* brokerIp, const int brokerPort);
		void loop1Hz();

	private:
	 	explicit JniMqttBroker();
	 	bool _reconnect();
		void _checkReconnect();
		void _handleMessage(char* topic, byte* payload, unsigned int length);
		uint8_t _aliveCounter = 0;

		WiFiClient _espClient;
		PubSubClient _client;
		long _lastReconnectAttempt = 0;

		static JniMqttBroker s_instance;
};
