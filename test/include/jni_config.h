#define SHOW_ENGINE_CONTROL_DEBUG 0
#define WIFI_SETUP 1

#if WIFI_SETUP == 1
	#define WIFI_SSID      "Loxodonta"
	#define WIFI_PASS      "Twitch7%Carton%Driller%Bluish"
#elif WIFI_SETUP == 2
	#define WIFI_SSID	  "Talpa"	
	#define WIFI_PASS	  "#K9bC5PRON3Lr3MQmJZtLgB#ax*Tr2kK96yb^83g&lfK%Te2F7Soai2TqPgJA&u"
#endif

#define UDP_RECEIVER_SOCKET_PORT 8080

#define JNI_MQTT_BROKER_IP "192.168.199.119"  // JNI's local MQTT broker
#define JNI_MQTT_BROKER_PORT 1883