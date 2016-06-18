#ifndef LAUMIOCONNECT_H
#define LAUMIOCONNECT_H

#include <ESP8266WiFi.h>

class LaumioConnect {
  public:
    LaumioConnect();

    void begin();

    // Wifi
    void setHostname(String hostname);
    bool isConnected();

    // AP
    void saveAP(String ssid, String pass);
    bool hasSavedAP();
    void connectToDefaultAP();
    void connectToSavedAP();

    String getAPName();

  private:
    void readAPFromEEPROM(String & ssid, String & pass);
    void saveAPToEEPROM(String ssid, String pass);
};

#endif                          /* LAUMIOCONNECT_H */
