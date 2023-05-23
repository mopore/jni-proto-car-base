#include "jni_input_reader.h"

InputReader::InputReader(const uint16_t udp_target_port) 
: m_udpReceiver(udp_target_port) {
}


void InputReader::setup() {
	m_udpReceiver.setup();
}


void InputReader::loop100Hz() {
	m_loopCounter++;
	if (m_loopCounter == 100){
		m_udpReceiver.loop1Hz();
		m_loopCounter = 0;
	}	
}
