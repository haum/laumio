#ifndef LAUMIOHTTP_H
#define LAUMIOHTTP_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class LaumioHttp {
  public:

    LaumioHttp();

    void begin();
    void handleClient();
    void handleRoot();

  private:

     ESP8266WebServer server;

    void handleNotFound();

    friend class LaumioApi;
};

#endif                          // LAUMIOHTTP_H
