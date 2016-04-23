#include "LaumioUdpRemoteControl.h"

LaumioUdpRemoteControl::LaumioUdpRemoteControl(LaumioLeds & l)
:  leds(l)
{
}

void LaumioUdpRemoteControl::begin()
{
    udpServer.begin(6969);

}

void LaumioUdpRemoteControl::interpretUdpMessage(char *buffer)
{
    if (buffer[0] == 0xff) {
        leds.fillColor(buffer[1], buffer[2], buffer[3]);
    } else {
        leds.setPixelColor(buffer[0], buffer[1], buffer[2], buffer[3]);
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
