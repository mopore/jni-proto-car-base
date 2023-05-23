#include "jni_shared_types.h"
#include <cstring>


volatile CarInput carInput;
volatile PowerStatus powerStatus;
volatile CarSensors carSensors;
volatile ConnectionSatus connectionStatus;


void setWifiStatusIP_v4(const char* ipv4) {
	// Cast away volatile qualifier to allow assignment
	ConnectionSatus *non_volatile_wifiStatus = const_cast<ConnectionSatus *>(&connectionStatus);
	strlcpy(non_volatile_wifiStatus->ip_v4, ipv4, MAX_IP_LENGTH);
}


const char* getWifiStatusIP_v4() {
	// Cast away volatile qualifier to allow access
	const ConnectionSatus *non_volatile_wifiStatus = const_cast<const ConnectionSatus *>(&connectionStatus);
	return non_volatile_wifiStatus->ip_v4;
}