/*
 * Connections:
 * D4 --- DIN
 * 3V3 -- 5VDC
 * G ---- GND (either)
 *
 * Adafruit recommendations:
 * - add a 1000uF capacitor across the power lines 5VDC and GND.
 * - add a 300-500 Ohm resistor on first pixel's data input DIN.
 * - avoid connecting on a live circuit... if you must, connect GND first.
 *
 * Dependencies:
 * https://github.com/adafruit/Adafruit_NeoPixel
 */
#define DIN_PIN D4
#define NUM_PIXELS 13

#include <ESP8266WiFi.h>
#include "wifi-config.h"

#include "LaumioLeds.h"

LaumioLeds leds(NUM_PIXELS, DIN_PIN);

void setup()
{
    Serial.begin(115200);
    leds.begin();
    delay(1000);
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
        laumio_state = ready;
        break;
    case ready:
        break;
    }
}
