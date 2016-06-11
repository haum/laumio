include "LaumioAP.h"

LaumioAP::LaumioAP(LaumioHttp& h) : server(h.server) {
}

void LaumioAP::begin(char* ssid, char const* pass) {

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);
    apip = WiFi.softAPIP();

    dns.start(53, "*", apip);

    server.on("/config", std::bind(&LaumioAP::handleConfig, this));
    server.onNotFound(std::bind(&LaumioAP::handleRedirectAP, this));
}

void LaumioAP::acceptDNS() {
    dns.processNextRequest();
}


void LaumioAP::handleConfig() {
    if (server.hostHeader() != apip.toString()) {
        handleRedirectAP();
        return;
    }

    server.send(200, "text/html", "<h1>Configuration de votre Laumio</h1>");
}

void LaumioAP::handleRedirectAP() {
    // Pas de cache pour ne pas casser la connexion de l'utilisateur lorsqu'il
    // se déconnecte du portail captif
    String header = "HTTP/1.1 301 OK\r\n"
            "Location: http://" + apip.toString() + "/config\r\n" +
            "Cache-Control: no-cache\r\n\r\n";

    server.sendContent(header);
}
