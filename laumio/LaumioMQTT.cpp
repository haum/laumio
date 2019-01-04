#include "LaumioMQTT.h"
#include "mqtt-config.h"

LaumioMQTT::LaumioMQTT(LaumioLeds & l, LaumioConnect & c)
:  espClient(c), leds(l)
{
}

void LaumioMQTT::begin()
{
	MQTTclient.setServer (mqtt_server, 1883);

}
