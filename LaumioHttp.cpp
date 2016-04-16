#include "LaumioHttp.h"

LaumioHttp::LaumioHttp()
:  server(80)
{
}

void LaumioHttp::begin()
{
    server.on("/", std::bind(&LaumioHttp::handleRoot, this));
    server.onNotFound(std::bind(&LaumioHttp::handleNotFound, this));
    server.begin();
}

void LaumioHttp::handleClient()
{
    server.handleClient();
}

void LaumioHttp::handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
}

void LaumioHttp::handleRoot()
{
    char root[500];
    char hostname[32];
    WiFi.hostname().toCharArray(hostname, sizeof(hostname));
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;

    snprintf(root, 500, "\
<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>LAUMio</title>\
    <style>\
      body { background-color: #f3f0ff; }\
    </style>\
  </head>\
  <body>\
    <h1>LAUMio</h1>\
    <p>Hostname: %s</p>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>\
", hostname, hr, min % 60, sec % 60);
    server.send(200, "text/html", root);
}
