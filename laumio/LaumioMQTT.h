#ifndef LAUMIOMQTT_H
#define LAUMIOMQTT_H


#include "LaumioLeds.h"
#include "LaumioConnect.h"

#include <PubSubClient.h>

class LaumioMQTT {
  public:
    LaumioMQTT(LaumioLeds & l, LaumioConnect & c);

    PubSubClient MQTTclient(LaumioConnect);

    void begin();



  private:

     LaumioConnect & espClient;
     LaumioLeds & leds;


};

#endif	/* LAUMIOMQTT_H */
