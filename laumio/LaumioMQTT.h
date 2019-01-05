#ifndef LAUMIOMQTT_H
#define LAUMIOMQTT_H


#include "LaumioLeds.h"
#include "LaumioConnect.h"

#include <Ethernet.h>
#include <PubSubClient.h>

class LaumioMQTT {
  public:
    LaumioMQTT(LaumioLeds & l);
    void begin();
    void loop();
    void callback(char* topic, byte* payload, unsigned int length);

  private:
     EthernetClient ethClient;
     PubSubClient client {ethClient};
     LaumioLeds & leds;


};

#endif	/* LAUMIOMQTT_H */
