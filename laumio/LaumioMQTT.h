#ifndef LAUMIOMQTT_H
#define LAUMIOMQTT_H


#include "LaumioLeds.h"
#include "LaumioConnect.h"

// #include <Ethernet.h>
#include <PubSubClient.h>

class LaumioMQTT {
  public:
    LaumioMQTT(LaumioLeds & l);
    void begin();
    void loop();
    void callback(char* topic, byte* payload, unsigned int length);

  private:
     WiFiClient ethClient;
     PubSubClient client {ethClient};
     LaumioLeds & leds;

     char NameString[16] = { 0 };
     long lastReconnectAttempt = 0;
};

#endif	/* LAUMIOMQTT_H */
