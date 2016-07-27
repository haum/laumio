#ifndef LAUMIOUDPREMOTECONTROL_H
#define LAUMIOUDPREMOTECONTROL_H

#include "LaumioLeds.h"
#include <WiFiUdp.h>

enum class Command {
    SetPixel = 0,
    SetRing = 1,
    AnimateRainbow = 0x0a,
    ColorWipe = 0x0b,
    Fill = 0xff
};

class LaumioUdpRemoteControl {
  public:

    LaumioUdpRemoteControl(LaumioLeds & l);

    void begin();
    void handleMessage();

  private:
     WiFiUDP udpServer;
     LaumioLeds & leds;

    void interpretUdpMessage(char *);
};

#endif                          // LAUMIOUDPREMOTECONTROL_H
