#ifndef LAUMIOAPI_H
#define LAUMIOAPI_H

#include "LaumioLeds.h"
#include "LaumioHttp.h"

#include <ArduinoJson.h>

class LaumioApi {
  public:

    LaumioApi(LaumioLeds & l, LaumioHttp & h);

    void begin();

  private:

     ESP8266WebServer & server;
     LaumioLeds & leds;

    void handleApi();
    void sendStatus();
    void interpretJson(JsonObject & jo);
};

#endif                          // LAUMIOAPI_H
