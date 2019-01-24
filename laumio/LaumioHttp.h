#ifndef LAUMIOHTTP_H
#define LAUMIOHTTP_H

#include "LaumioConfig.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

class LaumioHttp {
  public:
	LaumioHttp(LaumioConfig &c, ESP8266WebServer &s);

  private:
	ESP8266WebServer &server;
	LaumioConfig &config;

	void handleNotFound();
	void handleRoot();
};

#endif
