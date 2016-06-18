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

#include <ESP8266WiFi.h>
#include "wifi-config.h"

#include <ESP8266mDNS.h>

#include "LaumioLeds.h"
#include "LaumioHttp.h"
#include "LaumioApi.h"
#include "LaumioUdpRemoteControl.h"

LaumioLeds leds(NUM_PIXELS, DIN_PIN);
LaumioHttp httpServer;
LaumioApi api(leds, httpServer);
LaumioUdpRemoteControl udpRC(leds);

char hostString[16] = { 0 };

void setup()
{
    Serial.begin(115200);
    leds.begin();
    httpServer.begin();
    api.begin();
    delay(1000);

    sprintf(hostString, "Laumio_%06X", ESP.getChipId());
    Serial.print("Hostname: ");
    Serial.println(hostString);
    WiFi.hostname(hostString);
}

enum State { off, start, wifi_sta_connecting, wifi_sta_connected, ready };
State laumio_state = start;
State laumio_previous_state = off;

void loop()
{
    if (laumio_previous_state != laumio_state) {
        laumio_previous_state = laumio_state;

        switch (laumio_state) {
        case ready:
            leds.animate(LaumioLeds::Animation::Clear);
            break;

        case wifi_sta_connecting:
            Serial.println();
            Serial.print("Wi-Fi: Connecting to '");
            Serial.print(wifi_ssid);
            Serial.println("' ...");
            WiFi.begin(wifi_ssid, wifi_password);
            break;
        }
    }

    switch (laumio_state) {
    case start:
        leds.animate(LaumioLeds::Animation::Hello);

        laumio_state = wifi_sta_connecting;
        break;
    case wifi_sta_connecting:
        if (WiFi.status() != WL_CONNECTED) {
            leds.animate(LaumioLeds::Animation::Loading);
        } else {
            Serial.println("Wi-Fi: Connected.");
            laumio_state = wifi_sta_connected;
        }
        break;
    case wifi_sta_connected:
        leds.animate(LaumioLeds::Animation::Happy);
        udpRC.begin();
        MDNS.begin(hostString);
        laumio_state = ready;
        break;
    case ready:
        httpServer.handleClient();
        udpRC.handleMessage();
        break;
    }
}
