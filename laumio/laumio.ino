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
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
/* Perhaps other later */

#include "LaumioAP.h"
#include "LaumioConfig.h"
#include "LaumioHttp.h"
#include "LaumioHttpApi.h"
#include "LaumioHttpConfig.h"
#include "LaumioLeds.h"
#include "LaumioMQTT.h"
#include "LaumioUdpRemoteControl.h"
#include "ota_helpers.h"

LaumioLeds leds(NUM_PIXELS, DIN_PIN);
LaumioConfig config;

ESP8266WebServer webserver(80);
LaumioHttp http_handler(config, webserver);
LaumioHttpApi http_api_handler(leds, webserver);
LaumioHttpConfig http_config_path(config, webserver);

LaumioUdpRemoteControl udpRC(leds);
LaumioAP ap(webserver);

LaumioMQTT mqtt_client(leds, config);

int connectCounter = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
	leds.begin();
	config.readFromEEPROM();
	webserver.begin();
	MDNS.addService("http", "tcp", 80);
	delay(1000);

	if (!strcmp(config.hostname.value(), "")) {
		char hostString[14];
		sprintf(hostString, "Laumio_%06X", ESP.getChipId());
		config.hostname.setValue(hostString);
	}

	Serial.print("Hostname: ");
	Serial.println(config.hostname.value());

	WiFi.hostname(config.hostname.value());

	ota_setup(config.hostname.value());
}

enum State {
	off,
	start,
	wifi_sta_connecting,
	wifi_sta_connected,
	wifi_sta_abort,
	ready
};
State laumio_state = start;
State laumio_previous_state = off;

void loop() {
	ota_loop();

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
			Serial.print(config.connect_essid.value());
			Serial.println("' ...");

			WiFi.begin(config.connect_essid.value(),
			           config.connect_password.value());
			break;
		case wifi_sta_abort:
			leds.colorWipe(0xff3c00, 100);
			ap.begin(config.hostname.value(), "");
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
		MDNS.begin(config.hostname.value());
		mqtt_client.begin();
		laumio_state = ready;
		break;
	case wifi_sta_abort:
		ap.acceptDNS();
		webserver.handleClient();
	case ready:
		webserver.handleClient();
		udpRC.handleMessage();
		mqtt_client.loop();
		break;
	}
}
