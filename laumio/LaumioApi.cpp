#include "LaumioApi.h"

LaumioApi::LaumioApi(LaumioLeds & l, LaumioHttp & s)
:  server(s.server), leds(l)
{
}

void LaumioApi::begin()
{
    server.on("/api/", std::bind(&LaumioApi::handleApi, this));
    server.on("/api", std::bind(&LaumioApi::handleApi, this));
}

void LaumioApi::sendStatus()
{
    StaticJsonBuffer < 200 > jsonBuffer;        // Reserve memory space for json
    JsonObject & root = jsonBuffer.createObject();
    root["hostname"] = WiFi.hostname();
    root["version"] = "devel";
    String json;
    root.printTo(json);
    server.send(200, "application/json", json);
}

void LaumioApi::interpretJson(JsonObject & jo)
{
    StaticJsonBuffer < 200 > jsonBuffer;        // Reserve memory space for json
    JsonObject & root = jsonBuffer.createObject();
    root["name"] = WiFi.hostname();
    int status_code = 200;
    if (jo.containsKey("rgb")) {
        const int led = jo["led"];
        const int r = jo["rgb"][0];
        const int g = jo["rgb"][1];
        const int b = jo["rgb"][2];

        // if led = 255 set all LEDs to rgb value
        if (led == 255) {
            leds.fillColor(r, g, b);
        } else {
            leds.setPixelColor(led, r, g, b);
        }
        leds.show();
        root["status"] = "Success";
    } else {
        status_code = 400;
        root["status"] = "Invalid Request";
    }
    String json;
    root.printTo(json);
    server.send(status_code, "application/json", json);
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
            StaticJsonBuffer < 200 > jsonBufferResponse;        // Reserve memory space for json
            JsonObject & response = jsonBufferResponse.createObject();
            response["hostname"] = WiFi.hostname();
            response["status"] = "Invalid Request";
            response["message"] = "Unable to parse JSON";
            String jsonResponse;
            response.printTo(jsonResponse);
            server.send(400, "application/json", jsonResponse);
            return;
        }
        interpretJson(root);
    }
}
