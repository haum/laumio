#include "LaumioMQTT.h"

namespace { // All in this namespace is soup to make the lib call a
	        // member-function instead of global-space-function
LaumioMQTT *mqtt = nullptr;
void mqtt_callback(char *topic, byte *payload, unsigned int length) {
	if (mqtt)
		mqtt->callback(topic, payload, length);
}
} // namespace

LaumioMQTT::LaumioMQTT(LaumioLeds &l, LaumioConfig &c) : leds(l), config(c) {
	mqtt = this;
}

void LaumioMQTT::begin() {
	client.setServer(config.mqtt_host.value(), 1883);
	client.setCallback(mqtt_callback);
	strncpy(topicId, config.hostname.value(), sizeof(topicId));
}

void LaumioMQTT::loop() {
	if (!client.connected()) {
		long now = millis();
		if (now - lastReconnectAttemptDate > 10 * 1000) {
			lastReconnectAttemptDate = now;

			// Attempt to reconnect
			Serial.print("Attempting MQTT connection...");
			char laumioLastWillTopic[14 + 13];
			sprintf(laumioLastWillTopic, "laumio/status/%13s", topicId);
			if (client.connect(topicId, config.mqtt_user.value(),
			                   config.mqtt_password.value(),
			                   laumioLastWillTopic, 0, 1, "offline")) {
				Serial.println(" connected.");

				// Once connected, publish an announcement...
				client.publish(laumioLastWillTopic, "online", true);
				client.publish("laumio/status/advertise", topicId);

				// ... and resubscribe
				client.subscribe("laumio/all/#");
				char myTopicsWildcard[10 + 13];
				sprintf(myTopicsWildcard, "laumio/%13s/#", topicId);
				client.subscribe(myTopicsWildcard);

			} else {
				Serial.print(" failed. (state=");
				Serial.print(client.state());
				Serial.println(")");
			}

			if (client.connected()) {
				lastReconnectAttemptDate = 0;
			}
		}

	} else {
		// Client connected
		client.loop();
	}
}

void LaumioMQTT::callback(char *topic, byte *payload, unsigned int len) {
	// Display topic and payload
	Serial.print("[MQTT topic] ");
	Serial.print(topic);
	Serial.print(" : ");
	for (int i = 0; i < len; i++) {
		Serial.print((char)payload[i], HEX);
		Serial.print(' ');
	}
	Serial.println();

	// Extract command from topic name
	char *cmd = "";
	if (!strncmp(topic, "laumio/all/", 11)) {
		cmd = topic + 11;
	} else {
		char myTopicsStartWith[9 + 13];
		sprintf(myTopicsStartWith, "laumio/%13s/", topicId);
		if (!strncmp(topic, myTopicsStartWith, 8 + 13)) {
			cmd = topic + 8 + 13;
		}
	}

	// Execute command
	if (!strcmp("", cmd)) {
		// No command, or not for me, ignore

	} else if (!strcmp("set_pixel", cmd)) {
		if (len >= 4) {
			leds.setPixelColor(payload[0], payload[1], payload[2], payload[3]);
			leds.show();
		}

	} else if (!strcmp("set_ring", cmd)) {
		if (len >= 4) {
			leds.setRingColor(payload[0], payload[1], payload[2], payload[3]);
			leds.show();
		}

	} else if (!strcmp("set_column", cmd)) {
		if (len >= 4) {
			leds.setColumnColor(payload[0], payload[1], payload[2], payload[3]);
			leds.show();
		}

	} else if (!strcmp("color_wipe", cmd)) {
		if (len >= 4)
			leds.colorWipe(payload[0] << 16 | payload[1] << 8 | payload[2],
			               payload[3]);

	} else if (!strcmp("animate_rainbow", cmd)) {
		leds.rainbowCycle(1);

	} else if (!strcmp("fill", cmd)) {
		if (len >= 3) {
			leds.fillColor(payload[0], payload[1], payload[2]);
			leds.show();
		}

	} else if (!strcmp("json", cmd)) {
		char payload_cstr[len + 1];
		memcpy(payload_cstr, payload, len);
		payload_cstr[len + 1] = 0;
		leds.jsonCommands(payload_cstr);

	} else if (!strcmp("discover", cmd)) {
		client.publish("laumio/status/advertise", topicId);

	} else {
		Serial.print("Command not found: ");
		Serial.println(cmd);
	}
}
