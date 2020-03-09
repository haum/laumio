#include "LaumioHttpApi.h"

LaumioHttpApi::LaumioHttpApi(LaumioLeds &l, ESP8266WebServer &s)
    : server(s), leds(l) {
	server.on("/api/", std::bind(&LaumioHttpApi::handleApi, this));
	server.on("/api", std::bind(&LaumioHttpApi::handleApi, this));
}

void LaumioHttpApi::handleApi() {
	if (LaumioHttp::testCaptive(server))
		return;

	StaticJsonDocument<200> jsonBufferAnswer;
	JsonObject answer = jsonBufferAnswer.createNestedObject();
	int answerCode = 200;

	if (server.method() == HTTP_GET) {
		answer["hostname"] = WiFi.hostname();
		answer["version"] = "devel";

	} else {
		bool success = false;
		if (server.args() > 0) {
			StaticJsonDocument<200> jsonBufferQuery;
			JsonObject query = jsonBufferQuery.createNestedObject();
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
	serializeJson(answer, answerJsonStr);
	server.send(answerCode, "application/json", answerJsonStr);
}
