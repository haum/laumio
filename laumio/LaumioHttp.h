#ifndef LAUMIOHTTP_H
#define LAUMIOHTTP_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class LaumioHttp {
  public:
    LaumioHttp(ESP8266WebServer & s);

  private:
    ESP8266WebServer & server;

    void handleNotFound();
    void handleRoot();
};

#endif
