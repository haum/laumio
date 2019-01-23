#ifndef LAUMIOAPI_H
#define LAUMIOAPI_H

#include "LaumioLeds.h"
#include "LaumioHttp.h"

#include <ArduinoJson.h>

class LaumioHttpApi {
  public:

    LaumioHttpApi(LaumioLeds & l, LaumioHttp & h);

    void begin();

  private:

     ESP8266WebServer & server;
     LaumioLeds & leds;

    void handleApi();
};

#endif                          // LAUMIOAPI_H
