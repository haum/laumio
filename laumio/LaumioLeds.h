#ifndef LAUMIOLEDS_H
#define LAUMIOLEDS_H

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

class LaumioLeds {
  public:
	enum Led {
		A_Bottom = 0,
		A_Middle,
		A_Top,

		C_Top,
		C_Middle,
		C_Bottom,

		B_Bottom,
		B_Middle,
		B_Top,

		Top,

		D_Top,
		D_Middle,
		D_Bottom,

		NB
	};

	const static Led LowerRing[4];
	const static Led MiddleRing[4];
	const static Led UpperRing[4];

	const static Led AColumn[3];
	const static Led BColumn[3];
	const static Led CColumn[3];
	const static Led DColumn[3];

	enum Animation {
		Clear,
		Hello,
		Loading,
		Happy,
	};

	LaumioLeds(uint16_t n, uint8_t p);
	void begin();
	void animate(Animation a);
	void fillColor(const uint8_t &r, const uint8_t &g, const uint8_t &b);
	int count();
	void setPixelColor(const int led, const uint8_t &r, const uint8_t &g,
	                   const uint8_t &b);
	void setRingColor(int ring, uint8_t r, uint8_t g, uint8_t b);
	void setColumnColor(int column, uint8_t r, uint8_t g, uint8_t b);
	void show();

	void colorWipe(uint32_t c, uint8_t wait);
	void rainbowCycle(uint8_t wait);
	uint32_t wheel(byte wheelPos);

	bool jsonCommands(const char *str);
	bool jsonCommands(JsonObject jo);

  private:
	Adafruit_NeoPixel strip;
};

#endif // LAUMIOLEDS_H
