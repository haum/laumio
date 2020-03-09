#include "LaumioHttpConfig.h"

LaumioHttpConfig::LaumioHttpConfig(LaumioConfig &c, ESP8266WebServer &s)
    : server(s), config(c) {
	server.on("/config/", std::bind(&LaumioHttpConfig::handleConfig, this));
	server.on("/config", std::bind(&LaumioHttpConfig::handleConfig, this));
}

bool LaumioHttpConfig::jsonCommands(JsonObject jo) {
	String cmd = jo["command"];
	if (cmd == "config_connect") {
		config.connect_enabled.setValue(jo.containsKey("enabled"));
		config.connect_essid.setValue(jo["essid"]);
		config.connect_password.setValue(jo["password"]);
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_ap") {
		config.ap_enabled.setValue(jo.containsKey("enabled"));
		config.ap_essid.setValue(jo["essid"]);
		config.ap_password.setValue(jo["password"]);
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_apfallback") {
		config.apfallback_enabled.setValue(jo.containsKey("enabled"));
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_mqtt") {
		config.mqtt_enabled.setValue(jo.containsKey("enabled"));
		config.mqtt_host.setValue(jo["host"]);
		config.mqtt_user.setValue(jo["user"]);
		config.mqtt_password.setValue(jo["password"]);
		config.saveToEEPROM();
		return true;

	} else if (cmd == "reboot") {
		ESP.restart();
	}

	return false;
}

void LaumioHttpConfig::handleConfig() {
	if (LaumioHttp::testCaptive(server))
		return;

	StaticJsonDocument<200> jsonBufferAnswer;
	JsonObject answer = jsonBufferAnswer.createNestedObject();
	int answerCode = 200;

	bool success = false;
	if (server.args() > 0) {
		StaticJsonDocument<200> jsonBufferQuery;
		JsonObject query = jsonBufferQuery.createNestedObject();
		for (int i = 0; i < server.args(); i++) {
			query[server.argName(i)] = server.arg(i);
		}
		success = jsonCommands(query);

	} else if (server.hasArg("plain")) {
		StaticJsonDocument<800> jsonBuffer;
		auto deserializeStatus = deserializeJson(jsonBuffer, server.arg("plain"));
		success = (deserializeStatus == DeserializationError::Ok) && jsonCommands(jsonBuffer.as<JsonObject>());
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

	String answerJsonStr;
	serializeJson(answer, answerJsonStr);
	server.send(answerCode, "application/json", answerJsonStr);
}
