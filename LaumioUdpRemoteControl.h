#ifndef LAUMIOUDPREMOTECONTROL_H
#define LAUMIOUDPREMOTECONTROL_H

#include "LaumioLeds.h"
#include <WiFiUdp.h>

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
