#pragma once

#include <Arduino.h>
#include <AsyncUDP.h>
#include "jni_shared_types.h"

#define UDP_RECEIVER_PACKAGE_SIZE 4	


class JniUdpReceiver {
	public:
	 	explicit JniUdpReceiver(const uint16_t udp_target_port);
		void setup();
		void loop1Hz();
	private:
		AsyncUDP udp;
		const uint16_t m_udp_target_port;
		int m_package_counter = 0;
};
