#include "LaumioHttpConfig.h"

LaumioHttpConfig::LaumioHttpConfig(LaumioConfig &c, ESP8266WebServer &s)
    : server(s), config(c) {
	server.on("/config/", std::bind(&LaumioHttpConfig::handleConfig, this));
	server.on("/config", std::bind(&LaumioHttpConfig::handleConfig, this));
}

bool LaumioHttpConfig::jsonCommands(JsonObject &jo) {
	String cmd = jo["command"].asString();
	if (cmd == "config_connect") {
		config.connect_enabled.setValue(jo.containsKey("enabled"));
		config.connect_essid.setValue(jo["essid"].asString());
		config.connect_password.setValue(jo["password"].asString());
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_ap") {
		config.ap_enabled.setValue(jo.containsKey("enabled"));
		config.ap_essid.setValue(jo["essid"].asString());
		config.ap_password.setValue(jo["password"].asString());
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_apfallback") {
		config.apfallback_enabled.setValue(jo.containsKey("enabled"));
		config.saveToEEPROM();
		return true;

	} else if (cmd == "config_mqtt") {
		config.mqtt_enabled.setValue(jo.containsKey("enabled"));
		config.mqtt_host.setValue(jo["host"].asString());
		config.mqtt_user.setValue(jo["user"].asString());
		config.mqtt_password.setValue(jo["password"].asString());
		config.saveToEEPROM();
		return true;
	}
	return false;
}

void LaumioHttpConfig::handleConfig() {
	StaticJsonBuffer<200>
	    jsonBufferAnswer; // Reserve memory space for answer json
	JsonObject &answer = jsonBufferAnswer.createObject();
	int answerCode = 200;

	bool success = false;
	if (server.args() > 0) {
		StaticJsonBuffer<200> jsonBufferQuery;
		JsonObject &query = jsonBufferQuery.createObject();
		for (int i = 0; i < server.args(); i++) {
			query[server.argName(i)] = server.arg(i);
		}
		success = jsonCommands(query);

	} else if (server.hasArg("plain")) {
		StaticJsonBuffer<800> jsonBuffer;
		JsonObject &obj = jsonBuffer.parseObject(server.arg("plain"));
		success = obj.success() && jsonCommands(obj);
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
	answer.printTo(answerJsonStr);
	server.send(answerCode, "application/json", answerJsonStr);
}
