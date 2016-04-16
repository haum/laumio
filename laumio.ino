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

#include <Adafruit_NeoPixel.h>

#define DIN_PIN D4

#define NUM_PIXELS 13

#include <ESP8266WiFi.h>

#include "wifi-config.h"

Adafruit_NeoPixel strip =
Adafruit_NeoPixel(NUM_PIXELS, DIN_PIN, NEO_GRB + NEO_KHZ800);

int pause = 1000;

void setup()
{
    Serial.begin(115200);
    Serial.println(DIN_PIN);
    strip.begin();
    strip.show();               // Start with all pixels off
    delay(pause);
}

enum State { off, start, wifi_sta_connecting, wifi_stat_connected, ready };
State laumio_state = start;
State laumio_previous_state = off;

void loop()
{
    if (laumio_previous_state != laumio_state) {
        laumio_previous_state = laumio_state;

        switch (laumio_state) {
        case ready:
            colorWipe(strip.Color(0, 0, 0), 50);
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
        colorWipe(strip.Color(255, 0, 0), 50);
        colorWipe(strip.Color(0, 0, 0), 50);
        laumio_state = wifi_sta_connecting;
        break;
    case wifi_sta_connecting:
        if (WiFi.status() != WL_CONNECTED) {
            colorWipe(strip.Color(0, 255, 0), 50);
            colorWipe(strip.Color(0, 0, 0), 50);
        } else {
            Serial.println("Wi-Fi: Connected.");
            laumio_state = wifi_stat_connected;
        }
        break;
    case wifi_stat_connected:
        rainbowCycle(1);
        laumio_state = ready;
        break;
    case ready:
        break;
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    uint16_t i;
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) {     // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i,
                                Wheel(((i * 256 / strip.numPixels()) +
                                       j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
