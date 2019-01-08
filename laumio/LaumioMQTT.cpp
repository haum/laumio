#include "LaumioMQTT.h"
#include "mqtt-config.h"

namespace { // All in this namespace is soup to make the lib call a member-function instead of global-space-function
LaumioMQTT * mqtt = nullptr;
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (mqtt)
    mqtt->callback(topic, payload, length);
}
}

LaumioMQTT::LaumioMQTT(LaumioLeds & l) : leds(l) {
  mqtt = this;
}

void LaumioMQTT::begin() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
  sprintf(NameString, "Laumio_%06X", ESP.getChipId());
}

void LaumioMQTT::loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;

      // Attempt to reconnect
      Serial.print("Attempting MQTT connection...");
      if (client.connect(NameString, mqtt_user, mqtt_pass)) {
        Serial.println(" connected.");

        // Once connected, publish an announcement...
        client.publish("laumio/status/advertise", NameString);

        // ... and resubscribe
        client.subscribe("laumio/all/#");
        char myTopicsWildcard[10+13];
        sprintf(myTopicsWildcard, "laumio/%13s/#", NameString);
        client.subscribe(myTopicsWildcard);

      } else {
        Serial.print(" failed. (state=");
        Serial.print(client.state());
        Serial.println(")");
      }

      if (client.connected()) {
        Serial.println("MQTT Connected");
        lastReconnectAttempt = 0;
      }
    }

  } else {
    // Client connected
    client.loop();
  }
}

void LaumioMQTT::callback(char* topic, byte* payload, unsigned int len) {
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
  char * cmd = "";
  if (!strncmp(topic, "laumio/all/", 11)) {
    cmd = topic + 11;
  } else {
    char myTopicsStartWith[9+13];
    sprintf(myTopicsStartWith, "laumio/%13s/", NameString);
    if (!strncmp(topic, myTopicsStartWith, 8 + 13)) {
      cmd = topic + 8 + 13;
    }
  }

  // Execute command
  if (!strcmp("", cmd)) {
    // No command, or not for me, ignore
  } else if (!strcmp("set_pixel", cmd)) {
    if (len >= 4)
      leds.setPixelColor(payload[0], payload[1], payload[2], payload[3]);
  } else if (!strcmp("set_ring", cmd)) {
    if (len >= 4)
      leds.setRingColor(payload[0], payload[1], payload[2], payload[3]);
  } else if (!strcmp("set_column", cmd)) {
    if (len >= 4)
      leds.setColumnColor(payload[0], payload[1], payload[2], payload[3]);
  } else if (!strcmp("color_wipe", cmd)) {
    if (len >= 4)
      leds.colorWipe(payload[0]<<16 | payload[1]<<8 | payload[2], payload[3]);
  } else if (!strcmp("animate_rainbow", cmd)) {
    leds.rainbowCycle(1);
  } else if (!strcmp("fill", cmd)) {
    if (len >= 3)
      leds.fillColor(payload[0], payload[1], payload[2]);
  } else if (!strcmp("json", cmd)) {
    char payload_cstr[len+1];
    memcpy(payload_cstr, payload, len);
    payload_cstr[len+1] = 0;
    leds.jsonCommands(payload_cstr);
  } else if (!strcmp("discover", cmd)) {
    client.publish("laumio/status/advertise", NameString);
  } else {
    Serial.print("Command not found: ");
    Serial.println(cmd);
  }
}
