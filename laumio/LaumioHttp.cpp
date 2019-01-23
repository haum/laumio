#include "LaumioHttp.h"

#include <ESP8266mDNS.h>

LaumioHttp::LaumioHttp()
:  server(80)
{
}

void LaumioHttp::begin()
{
    server.on("/", std::bind(&LaumioHttp::handleRoot, this));
    server.onNotFound(std::bind(&LaumioHttp::handleNotFound, this));
    server.begin();
    MDNS.addService("http", "tcp", 80);
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
    const auto html = R"(
<html>
	<head>
		<title>Laumio</title>
		<style>
			body { background: #eee; padding: 0; }
			section {
				margin: 0.1%;
				padding: 0.4%;
				float: left;
				width: 26%;
				height: 99%;
				background: #fff;
			}
			section.wide { width: 45%; }
			h1 { font-size: x-large; color: #CC8A4D; }
			h2 { font-size: large; color: #4D97CC; }
			a { color: #93859D; }
			@media (max-width: 835px) {
				section, section.wide {
					float: none;
					width: 99%;
					height: auto;
				}
			}
		</style>
	</head>
	<body>
		<section>
			<h1>Laumio</h1>
			<h2>Hostname</h2>
			<input type="text" disabled="" value="^" />
			<h2>Uptime</h2>
			<input type="text" disabled="" value="^" />
			<h2>Links</h2>
			<p><a href="https://haum.org/pages/laumios.html">Project page</a></p>
			<p><a href="https://github.com/haum/laumio/">Source code</a></p>
			<p><a href="https://laumio.readthedocs.io/en/latest/">Documentation</a></p>
		</section>
		<section class="wide">
			<h1>Control</h1>
			<h2>Set pixel</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="set_pixel" />
				<input name="led" type="number" min="0" max="13" placeholder="Led number" title="Led number" />
				<input name="rgb" type="color" title="Color" />
				<input type="submit" />
			</form>
			<h2>Set ring</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="set_ring" />
				<input name="ring" type="number" min="0" max="2" placeholder="Ring number" title="Ring number" />
				<input name="rgb" type="color" title="Color" />
				<input type="submit" />
			</form>
			<h2>Set column</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="set_column" />
				<input name="column" type="number" min="0" max="3" placeholder="Column number" title="Column number" />
				<input name="rgb" type="color" title="Color" />
				<input type="submit" />
			</form>
			<h2>Color wipe</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="color_wipe" />
				<input name="duration" type="number" min="0" placeholder="Duration" title="Duration" />
				<input name="rgb" type="color" title="Color" />
				<input type="submit" />
			</form>
			<h2>Animate rainbow</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="animate_rainbow" />
				<input type="submit" />
			</form>
			<h2>Fill</h2>
			<form method="post" action="/api/">
				<input name="command" type="hidden" value="fill" />
				<input name="rgb" type="color" title="Color" />
				<input type="submit" />
			</form>
		</section>
		<section>
			<h1>Config</h1>
			<p>No config yet.</p>
		</section>
	</body>
</html>)";

    auto dynData = [](char * buffer, int buffersz, int nb) {
        switch(nb) {
            case 0:
                WiFi.hostname().toCharArray(buffer, buffersz);
                break;

            case 1:
                int sec = millis() / 1000;
                int min = sec / 60;
                int hr = min / 60;
                sprintf(buffer, "%02d:%02d:%02d", hr, min % 60, sec % 60);
                break;
        }
    };

    // Send html string, and call dynData(...) to replace each '^'.
    char buffer[512];
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", ""); // Send headers
    int dynDataNb = 0;
    const char * const htmlend = html + strlen(html);
    const char * htmlsend = html;
    const char * it = htmlsend - 1;
    while (htmlsend < htmlend - 1) {
        if (it <= htmlsend + 1) {
            it = std::find(it+1, htmlend, '^');
        }
        int vol = it - htmlsend - 1;
        vol = std::min(vol, int(sizeof(buffer)-1));
        memcpy(buffer, htmlsend + 1, vol);
        buffer[vol] = 0;
        htmlsend += vol;
        server.sendContent(buffer);
        if (it <= htmlsend + 1 && it != htmlend) {
            buffer[0] = 0;
            dynData(buffer, sizeof(buffer), dynDataNb);
            dynDataNb++;
            server.sendContent(buffer);
            htmlsend += 1;
        }
    }

}
