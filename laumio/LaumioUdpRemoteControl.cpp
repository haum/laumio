#include "LaumioUdpRemoteControl.h"

#include <ESP8266mDNS.h>

LaumioUdpRemoteControl::LaumioUdpRemoteControl(LaumioLeds &l) : leds(l) {}

void LaumioUdpRemoteControl::begin() {
	udpServer.begin(6969);
	MDNS.addService("laumiorc", "udp", 6969);
}

void LaumioUdpRemoteControl::interpretUdpMessage(char *buffer, int len) {
	while (len > 0) {
		unsigned int cmdsize = 1;
		Command command = static_cast<Command>(buffer[0]);

		switch (command) {

		case Command::SetPixel:
			cmdsize = 5;
			if (len >= cmdsize)
				leds.setPixelColor(buffer[1], buffer[2], buffer[3], buffer[4]);
			break;

		case Command::SetRing:
			cmdsize = 5;
			if (len >= cmdsize)
				leds.setRingColor(buffer[1], buffer[2], buffer[3], buffer[4]);
			break;

		case Command::SetColumn:
			cmdsize = 5;
			if (len >= cmdsize)
				leds.setColumnColor(buffer[1], buffer[2], buffer[3], buffer[4]);
			break;

		case Command::ColorWipe:
			cmdsize = 5;
			if (len >= cmdsize)
				leds.colorWipe(buffer[1] << 16 | buffer[2] << 8 | buffer[3],
				               buffer[4]);
			break;

		case Command::AnimateRainbow:
			cmdsize = 1;
			leds.rainbowCycle(1);
			break;

		case Command::Fill:
			cmdsize = 4;
			if (len >= cmdsize)
				leds.fillColor(buffer[1], buffer[2], buffer[3]);
			break;
		}

		len -= cmdsize;
		buffer += cmdsize;
	}
	leds.show();
}

void LaumioUdpRemoteControl::handleMessage() {
	char buffer[255]; // buffer to hold incoming packet
	// if there's data available, read a packet
	int packetSize = udpServer.parsePacket();
	if (packetSize) {
		int len = udpServer.read(buffer, 255);
		if (len > 0)
			interpretUdpMessage(buffer, len);
	}
}
