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

DNSServer dns;

LaumioUdpRemoteControl udpRC(leds);
LaumioMQTT mqtt_client(leds, config);

bool connect_enabled;
bool ap_enabled;
bool apfallback_enabled;
bool mqtt_enabled;

void setup() {
	Serial.begin(115200);
	Serial.println("Starting Laumio\n");

	config.readFromEEPROM();
	if (!strcmp(config.hostname, "")) {
		char hostString[14];
		sprintf(hostString, "Laumio_%06X", ESP.getChipId());
		config.hostname.setValue(hostString);
	}

	connect_enabled = config.connect_enabled;
	ap_enabled = config.ap_enabled;
	apfallback_enabled = config.apfallback_enabled;
	mqtt_enabled = config.mqtt_enabled;

	Serial.print("Hostname: ");
	Serial.println(config.hostname);

	leds.begin();
	leds.animate(LaumioLeds::Animation::Clear);
	leds.animate(LaumioLeds::Animation::Hello);

	 
	webserver.begin();
  // MDNS.addService("http", "tcp", 80);
  WiFi.hostname(config.hostname);
	ota_setup(config.hostname);

	Serial.print("Connection to wifi");
	if (config.connect_enabled && strcmp(config.connect_essid, "")) {
		Serial.print(": \"");
		Serial.print(config.connect_essid);
		Serial.println('"');
		WiFi.begin(config.connect_essid, config.connect_password);
	} else {
		Serial.println(" disabled");
	}

	Serial.print("Wifi access point");
	if (config.ap_enabled && strcmp(config.ap_essid, "")) {
		Serial.print(": \"");
		Serial.print(config.ap_essid);
		Serial.print('"');
		if (WiFi.softAP(config.ap_essid, config.ap_password)) {
			Serial.print(' ');
			Serial.println(WiFi.softAPIP());
			dns.start(53, "*", WiFi.softAPIP());
		} else {
			Serial.println(" failed");
			ap_enabled = false;
		}
	} else {
		WiFi.softAPdisconnect();
		Serial.println(" disabled");
	}

	if (mqtt_enabled)
		mqtt_client.begin();
	udpRC.begin();
	MDNS.begin(config.hostname.value());
  /**/
}

void loop() {
	ota_loop();
	dns.processNextRequest();
	webserver.handleClient();
	udpRC.handleMessage();
	if (mqtt_enabled)
		mqtt_client.loop();

	// Connection indicator
	static bool connectionFailed = false;
	if (connect_enabled && !connectionFailed) {
		static int connectCounter = 0;
		if (WiFi.status() != WL_CONNECTED) {
			if (connectCounter == 10) {
				leds.colorWipe(0xff3c00, 100);
				connectCounter++;
				WiFi.disconnect();
				Serial.println("Connection failed (too long)");
				connectionFailed = true;
			} else {
				leds.animate(LaumioLeds::Animation::Loading);
				connectCounter++;
			}
		} else if (connectCounter != -1) {
			Serial.print("Wifi connected: ");
			Serial.println(WiFi.localIP());
			leds.animate(LaumioLeds::Animation::Happy);
			leds.animate(LaumioLeds::Animation::Clear);
			connectCounter = -1;
		}
	} else {
		connectionFailed = true;
	}

	// Fallback
	if (connectionFailed && !ap_enabled) {
		if (apfallback_enabled) {
			Serial.print("Access point fallback enabled: \"");
			Serial.print(config.hostname);
			Serial.print('"');

			if (WiFi.softAP(config.hostname, "")) {
				Serial.print(' ');
				Serial.println(WiFi.softAPIP());
				dns.start(53, "*", WiFi.softAPIP());
				ap_enabled = true;
			} else {
				Serial.println(" failed");
				delay(3000);
				ESP.restart();
			}
		} else {
			delay(3000);
			ESP.restart();
		}
	}
}
