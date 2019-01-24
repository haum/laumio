#ifndef LAUMIOMQTT_H
#define LAUMIOMQTT_H

#include "LaumioLeds.h"
#include "LaumioConfig.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class LaumioMQTT {
  public:
    LaumioMQTT(LaumioLeds & l, LaumioConfig & c);
    void begin();
    void loop();
    void callback(char* topic, byte* payload, unsigned int length);

  private:
     WiFiClient ethClient;
     PubSubClient client {ethClient};
     LaumioLeds & leds;
     LaumioConfig & config;

     char NameString[32] = { 0 };
     long lastReconnectAttempt = 0;
};

#endif	/* LAUMIOMQTT_H */
