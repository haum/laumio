#ifndef LAUMIOHTTPCONFIG_H
#define LAUMIOHTTPCONFIG_H

#include "LaumioConfig.h"
#include "LaumioHttp.h"

#include <ArduinoJson.h>

class LaumioHttpConfig {
  public:
	LaumioHttpConfig(LaumioConfig &c, ESP8266WebServer &s);

  private:
	ESP8266WebServer &server;
	LaumioConfig &config;

	bool jsonCommands(JsonObject jo);
	void handleConfig();
};

#endif
