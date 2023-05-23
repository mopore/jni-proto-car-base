#pragma once
#include <Arduino.h>
#include "jni_shared_types.h"
#include "jni_udp_receiver.h"


class InputReader {
	public:
		explicit InputReader(const uint16_t udp_target_port);
		void setup();
		void loop100Hz();
	private:
		JniUdpReceiver m_udpReceiver;
		int8_t m_loopCounter = 0;
};