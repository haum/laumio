#ifndef LAUMIOMQTT_H
#define LAUMIOMQTT_H

#include "LaumioConfig.h"
#include "LaumioLeds.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class LaumioMQTT {
  public:
	LaumioMQTT(LaumioLeds &l, LaumioConfig &c);
	void begin();
	void loop();
	void callback(char *topic, byte *payload, unsigned int length);

  private:
	WiFiClient ethClient;
	PubSubClient client{ethClient};
	LaumioLeds &leds;
	LaumioConfig &config;

	char topicId[32] = {0};
	long lastReconnectAttemptDate = 0;
};

#endif /* LAUMIOMQTT_H */
