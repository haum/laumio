#include "LaumioApi.h"

LaumioApi::LaumioApi(LaumioLeds & l, LaumioHttp & s)
:  server(s.server), leds(l)
{
}

void LaumioApi::begin()
{
    server.on("/api/", std::bind(&LaumioApi::handleApi, this));
}

void LaumioApi::sendStatus()
{
    StaticJsonBuffer < 200 > jsonBuffer;        // Reserve memory space for json
    JsonObject & root = jsonBuffer.createObject();
    root["name"] = "laumio";
    root["version"] = "devel";
    String json;
    root.printTo(json);
    server.send(200, "text/plain", json);
}

void LaumioApi::interpretJson(JsonObject & jo)
{
    if (jo.containsKey("rgb")) {
        const int led = jo["led"];
        const int r = jo["rgb"][0];
        const int g = jo["rgb"][1];
        const int b = jo["rgb"][2];

        // if led = 255 set all LEDs to rgb value
        if (led == 255) {
            leds.fill(r, g, b);
        } else if (led < leds.count()) {
            leds.setPixelColor(led, r, g, b);
        }
        leds.show();
    }
    server.send(200, "text/plain", "ok");
}


void LaumioApi::handleApi()
{
    if (server.method() == HTTP_GET) {
        sendStatus();
        return;
    } else {
        String json;
        if (server.hasArg("plain")) {
            json = server.arg("plain");
        }
        // Deserialize the JSON string
        StaticJsonBuffer < 200 > jsonBuffer;    // Reserve memory space for json
        JsonObject & root = jsonBuffer.parseObject(json);
        if (!root.success()) {
            server.send(400, "text/plain", "parseObject() failed");
            return;
        }
        interpretJson(root);
    }
}
