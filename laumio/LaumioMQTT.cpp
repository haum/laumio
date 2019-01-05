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
  client.subscribe("laumio/all/#");
}

void LaumioMQTT::loop() {
  client.loop();
}

void LaumioMQTT::callback(char* topic, byte* payload, unsigned int len) {
  // Extract command from topic name
  char * cmd = "";
  if (!strcmp(topic, "laumio/all/")) {
    cmd = topic + 11;
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
  }
}
