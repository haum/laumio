#include "ota_helpers.h"

#include <ArduinoOTA.h>

void ota_setup(const char *hostname) {
	ArduinoOTA.setHostname(hostname);

	ArduinoOTA.onStart([] {
		Serial.print("OTA: Start updating ");
		Serial.println((ArduinoOTA.getCommand() == U_FLASH) ? "sketch"
		                                                    : "filesystem");
	});

	ArduinoOTA.onEnd([] { Serial.println("\nOTA: End"); });

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("OTA: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("OTA: Error[%u], ", error);
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

	ArduinoOTA.begin();
}

void ota_loop() { ArduinoOTA.handle(); }
