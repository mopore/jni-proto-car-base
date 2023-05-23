#include "jni_udp_receiver.h"

JniUdpReceiver::JniUdpReceiver(
	const uint16_t udp_target_port
):
	m_udp_target_port(udp_target_port){
}

void JniUdpReceiver::setup() {
	byte packetBuffer[UDP_RECEIVER_PACKAGE_SIZE];
	if (udp.listen(m_udp_target_port)) {
		Serial.println("Listening...");
		// Aysnchronously receive UDP packets
		udp.onPacket([this](AsyncUDPPacket packet) {
			if (packet.length() == UDP_RECEIVER_PACKAGE_SIZE) {
				int16_t x = packet.data()[0] | packet.data()[1] << 8;
				int16_t y = packet.data()[2] | packet.data()[3] << 8;
				carInput.x = x;
				carInput.y = y;
				m_package_counter++;
			}
		});
	}
}


void JniUdpReceiver::loop1Hz() {
	connectionStatus.isUdpWorking = (m_package_counter > 0) ? true : false; 
	m_package_counter = 0;
}
