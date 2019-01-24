#ifndef LAUMIOAPI_H
#define LAUMIOAPI_H

#include "LaumioHttp.h"
#include "LaumioLeds.h"

#include <ArduinoJson.h>

class LaumioHttpApi {
  public:
	LaumioHttpApi(LaumioLeds &l, ESP8266WebServer &s);

  private:
	ESP8266WebServer &server;
	LaumioLeds &leds;

	void handleApi();
};

#endif
