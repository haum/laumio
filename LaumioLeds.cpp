#include "LaumioLeds.h"

LaumioLeds::LaumioLeds(uint16_t n, uint8_t p)
:  strip(n, p)
{
}

void LaumioLeds::begin()
{
    strip.begin();
    strip.show();
}

void LaumioLeds::animate(Animation a)
{
    switch (a) {
    case Animation::Clear:
        colorWipe(strip.Color(0, 0, 0), 50);
        break;
    case Animation::Hello:
        colorWipe(strip.Color(255, 0, 255), 50);
        colorWipe(strip.Color(0, 0, 0), 50);
        break;
    case Animation::Loading:
        colorWipe(strip.Color(255, 0, 0), 50);
        colorWipe(strip.Color(0, 0, 0), 50);
        break;
    case Animation::Happy:
	rainbowCycle(1);
        break;
    }
}

// Fill the dots one after the other with a color
void LaumioLeds::colorWipe(uint32_t c, uint8_t wait)
{
    uint16_t i;
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void LaumioLeds::rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) {     // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i,
                                Wheel(((i * 256 / strip.numPixels()) +
                                       j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LaumioLeds::Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
