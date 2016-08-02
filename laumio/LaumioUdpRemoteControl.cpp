#include "LaumioUdpRemoteControl.h"

#include <ESP8266mDNS.h>

LaumioUdpRemoteControl::LaumioUdpRemoteControl(LaumioLeds & l)
:  leds(l)
{
}

void LaumioUdpRemoteControl::begin()
{
    udpServer.begin(6969);
    MDNS.addService("laumiorc", "udp", 6969);
}

void LaumioUdpRemoteControl::interpretUdpMessage(char *buffer)
{
    Command command = static_cast < Command > (buffer[0]);
    uint8_t wait = 100;

    switch (command) {

    case Command::SetPixel:
        leds.setPixelColor(buffer[1], buffer[2], buffer[3], buffer[4]);
        break;

    case Command::SetRing:
        leds.setRingColor(buffer[1], buffer[2], buffer[3], buffer[4]);
        break;

    case Command::SetColumn:
        leds.setColumnColor(buffer[1], buffer[2], buffer[3], buffer[4]);
        break;

    case Command::ColorWipe:
        if (sizeof(buffer)/sizeof(char) > 3) {
          wait = buffer[4];
        }
        leds.colorWipe(buffer[1]<<16 | buffer[2]<<8 | buffer[3], wait);
        break;

    case Command::AnimateRainbow:
        leds.rainbowCycle(1);
        break;

    case Command::Fill:
        leds.fillColor(buffer[1], buffer[2], buffer[3]);
        break;
    }

    leds.show();
}

void LaumioUdpRemoteControl::handleMessage()
{
    char buffer[255];           //buffer to hold incoming packet
    // if there's data available, read a packet
    int packetSize = udpServer.parsePacket();
    if (packetSize) {
        int len = udpServer.read(buffer, 255);
        if (len > 0) {
            buffer[len] = '\0';
            interpretUdpMessage(buffer);
        }
    }
}
