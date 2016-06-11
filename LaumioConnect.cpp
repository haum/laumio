#include "LaumioConnect.h"

#include <EEPROM.h>

#include "wifi-config.h"

LaumioConnect::LaumioConnect() {
}

void LaumioConnect::begin() {
    if (hasSavedAP()) {
        connectToSavedAP();
    } else {
        connectToDefaultAP();
    }
}

// Wifi

void LaumioConnect::setHostname(String hostname) {
    WiFi.hostname(hostname);
}


bool LaumioConnect::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// AP

void LaumioConnect::saveAP(String ssid, String pass) {
    saveAPToEEPROM(ssid, pass);
}

bool LaumioConnect::hasSavedAP() {
    String ssid, pass;
    readAPFromEEPROM(ssid, pass);
    
    if (ssid.length() > 1) return true;
    
    return false;
}

void LaumioConnect::connectToSavedAP() {
    String ssid, pass;
    readAPFromEEPROM(ssid, pass);
    
    WiFi.begin(ssid.c_str(), pass.c_str());
}

void LaumioConnect::connectToDefaultAP() {
    WiFi.begin(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
}

String LaumioConnect::getAPName() {
    if (hasSavedAP()) {
        String ssid, pass;
        readAPFromEEPROM(ssid, pass);
        
        return ssid;
    }
    
    return DEFAULT_WIFI_SSID;
}


// Private helpers

void LaumioConnect::readAPFromEEPROM(String& ssid, String& pass) {
    
    ssid = "";
    for (int i = 0; i < 32; ++i) {
        ssid += char(EEPROM.read(i));
    }
    
    pass = "";
    
    for (int i = 32; i < 96; ++i) {
        pass += char(EEPROM.read(i));
    }
}

void LaumioConnect::saveAPToEEPROM(String ssid, String pass) {

    // Clear
    for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
    }

    for (int i = 0; i < ssid.length(); ++i) {
        EEPROM.write(i, ssid[i]);
    }

    for (int i = 0; i < pass.length(); ++i) {
        EEPROM.write(32 + i, pass[i]);
    }
    
    EEPROM.commit();
}