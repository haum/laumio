#include "LaumioHttpApi.h"

LaumioHttpApi::LaumioHttpApi(LaumioLeds & l, LaumioHttp & s)
:  server(s.server), leds(l)
{
}

void LaumioHttpApi::begin()
{
    server.on("/api/", std::bind(&LaumioHttpApi::handleApi, this));
    server.on("/api", std::bind(&LaumioHttpApi::handleApi, this));
}

void LaumioHttpApi::handleApi()
{
    StaticJsonBuffer <200> jsonBufferAnswer; // Reserve memory space for answer json
    JsonObject & answer = jsonBufferAnswer.createObject();
    int answerCode = 200;

    if (server.method() == HTTP_GET) {
        answer["hostname"] = WiFi.hostname();
        answer["version"] = "devel";

    } else {
        bool success = false;
        if (server.args() > 1) {
            StaticJsonBuffer <200> jsonBufferQuery;
            JsonObject & query = jsonBufferQuery.createObject();
            for (int i = 0; i < server.args(); i++) {
                query[server.argName(i)] = server.arg(i);
            }
            success = leds.jsonCommands(query);

        } else if (server.hasArg("plain")) {
            String jsonRequestStr = server.arg("plain");
            success = leds.jsonCommands(jsonRequestStr.c_str());
        }

        if (success) {
            answer["hostname"] = WiFi.hostname();
            answer["status"] = "Success";

        } else {
            answer["hostname"] = WiFi.hostname();
            answer["status"] = "Invalid Request";
            answer["message"] = "Unable to parse JSON";
            answerCode = 400;
        }
    }

    String answerJsonStr;
    answer.printTo(answerJsonStr);
    server.send(answerCode, "application/json", answerJsonStr);
}
