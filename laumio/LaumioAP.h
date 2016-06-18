#ifndef LAUMIOAP_H
#define LAUMIOAP_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "LaumioHttp.h"

class LaumioAP {
public:
    LaumioAP(LaumioHttp& h);

    void begin(char *ssid, char const* pass);
    void acceptDNS();


private:

    void handleConfig();
    void handleRedirectAP();

    IPAddress apip;
    DNSServer dns;
    ESP8266WebServer& server;

};

#endif /* LAUMIOAP_H */
