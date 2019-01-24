#include "LaumioAP.h"

LaumioAP::LaumioAP(ESP8266WebServer &s) : server(s) {}

void LaumioAP::begin(char *ssid, char const *pass) {

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, pass);
	apip = WiFi.softAPIP();

	dns.start(53, "*", apip);

	server.onNotFound(std::bind(&LaumioAP::handleRedirectAP, this));
}

void LaumioAP::acceptDNS() { dns.processNextRequest(); }

void LaumioAP::handleRedirectAP() {
	// Pas de cache pour ne pas casser la connexion de l'utilisateur lorsqu'il
	// se déconnecte du portail captif
	String header = String("HTTP/1.1 301 OK\r\n") +
	                String("Location: http://") + apip.toString() +
	                String("/hello\r\n") +
	                String("Cache-Control: no-cache\r\n\r\n");

	server.sendContent(header);
}
