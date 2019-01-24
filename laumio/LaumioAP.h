#ifndef LAUMIOAP_H
#define LAUMIOAP_H

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "LaumioHttp.h"

class LaumioAP {
  public:
	LaumioAP(ESP8266WebServer &s);

	void begin(char *ssid, char const *pass);
	void acceptDNS();

  private:
	void handleHello();
	void handleRedirectAP();

	IPAddress apip;
	DNSServer dns;
	ESP8266WebServer &server;
};

#endif /* LAUMIOAP_H */
