/*
 * Connections:
 * D3 --- DIN
 * G ---- GND (either)
 *
 * Adafruit recommendations:
 * - add a 1000uF capacitor across the power lines 5VDC and GND.
 * - add a 300-500 Ohm resistor on first pixel's data input DIN.
 * - avoid connecting on a live circuit... if you must, connect GND first.
 *
 * Dependencies:
 * https://github.com/adafruit/Adafruit_NeoPixel
 * https://github.com/bblanchon/ArduinoJson
 */

// Si ancienne version
//#define DIN_PIN D4
#define DIN_PIN D3

#define NUM_PIXELS 13

/* Old versions of Arduino pseudo-IDE need these to compile the code **/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
/* Perhaps other later */

#include "LaumioAP.h"
#include "LaumioLeds.h"
#include "LaumioHttp.h"
#include "LaumioHttpApi.h"
#include "LaumioUdpRemoteControl.h"
#include "LaumioMQTT.h"

LaumioLeds leds(NUM_PIXELS, DIN_PIN);
LaumioHttp httpServer;
LaumioHttpApi api(leds, httpServer);
LaumioUdpRemoteControl udpRC(leds);
LaumioAP ap(httpServer);

LaumioMQTT mqtt_client(leds);

int connectCounter = 0;

char config_ap_password[32] = "";
char config_hostname[32] = "";
char config_connect_essid[32] = "";
char config_connect_password[32] = "";

void setup()
{
    Serial.begin(115200);
    Serial.println();
    leds.begin();
    httpServer.begin();
    api.begin();
    delay(1000);

    char hostString[14];
    sprintf(hostString, "Laumio_%06X", ESP.getChipId());

    if (!strcmp(config_hostname, ""))
        strcpy(config_hostname, hostString);

    Serial.print("Hostname: ");
    Serial.println(config_hostname);

    WiFi.hostname(config_hostname);

    ArduinoOTA.setHostname(config_hostname);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

}

enum State { off, start, wifi_sta_connecting, wifi_sta_connected,
        wifi_sta_abort, ready };
State laumio_state = start;
State laumio_previous_state = off;



void loop()
{
ArduinoOTA.handle();

    // Changement d'état
    if (laumio_previous_state != laumio_state) {
        laumio_previous_state = laumio_state;

        switch (laumio_state) {
        case ready:
            leds.animate(LaumioLeds::Animation::Clear);
            break;

        case wifi_sta_connecting:
            Serial.println();
            Serial.print("Wi-Fi: Connecting to '");
            Serial.print(config_connect_essid);
            Serial.println("' ...");

            WiFi.begin(config_connect_essid, config_connect_password);
            break;
        case wifi_sta_abort:
            leds.colorWipe(0xff3c00, 100);
            ap.begin(config_hostname, config_ap_password);
            break;
        }
    }
    // État
    switch (laumio_state) {
    case start:
        leds.animate(LaumioLeds::Animation::Hello);

        laumio_state = wifi_sta_connecting;
        break;
    case wifi_sta_connecting:
        if (WiFi.status() != WL_CONNECTED) {
            leds.animate(LaumioLeds::Animation::Loading);
            connectCounter++;

            if (connectCounter > 10) {
                laumio_state = wifi_sta_abort;
                Serial.println("Wi-Fi: Too long, abort.");
            }
        } else {
            Serial.println("Wi-Fi: Connected.");
            Serial.print("IP Address : ");
            Serial.println(WiFi.localIP());
            laumio_state = wifi_sta_connected;
        }
        break;
    case wifi_sta_connected:
        leds.animate(LaumioLeds::Animation::Happy);
        udpRC.begin();
        MDNS.begin(config_hostname);
        mqtt_client.begin();
        ArduinoOTA.begin();
        laumio_state = ready;
        break;
    case wifi_sta_abort:
        ap.acceptDNS();
        httpServer.handleClient();
    case ready:
        httpServer.handleClient();
        udpRC.handleMessage();
        mqtt_client.loop();
        break;
    }
}
