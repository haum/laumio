#include "LaumioLeds.h"

const LaumioLeds::Led LaumioLeds::LowerRing[4] = {
    A_Bottom,
    B_Bottom,
    C_Bottom,
    D_Bottom
};

const LaumioLeds::Led LaumioLeds::MiddleRing[4] = {
    A_Middle,
    B_Middle,
    C_Middle,
    D_Middle
};

const LaumioLeds::Led LaumioLeds::UpperRing[4] = {
    A_Top,
    B_Top,
    C_Top,
    D_Top
};

const LaumioLeds::Led LaumioLeds::AColumn[3] = {
    A_Bottom,
    A_Middle,
    A_Top
};

const LaumioLeds::Led LaumioLeds::BColumn[3] = {
    B_Bottom,
    B_Middle,
    B_Top
};

const LaumioLeds::Led LaumioLeds::CColumn[3] = {
    C_Bottom,
    C_Middle,
    C_Top
};

const LaumioLeds::Led LaumioLeds::DColumn[3] = {
    D_Bottom,
    D_Middle,
    D_Top
};

LaumioLeds::LaumioLeds(uint16_t n, uint8_t p)
:  strip(n, p)
{
}

void LaumioLeds::begin()
{
    strip.Begin();
    strip.Show();
}

int LaumioLeds::count()
{
    return strip.PixelCount();
}

void LaumioLeds::setPixelColor(const int led, const uint8_t & r,
                               const uint8_t & g, const uint8_t & b)
{
    if (led < strip.PixelCount()) {
        strip.SetPixelColor(led, RgbColor(r, g, b));
    }
}

void LaumioLeds::setRingColor(int ring, uint8_t r, uint8_t g, uint8_t b)
{
    LaumioLeds::Led * pring = nullptr;
    switch (ring) {
    case 0:
        pring = (LaumioLeds::Led *) & LaumioLeds::LowerRing;
        break;
    case 1:
        pring = (LaumioLeds::Led *) & LaumioLeds::MiddleRing;
        break;
    case 2:
        pring = (LaumioLeds::Led *) & LaumioLeds::UpperRing;
        break;
    }

    if (pring) {
        for (int i = 0; i < 4; i++) {
            strip.SetPixelColor(pring[i], RgbColor(r, g, b));
        }
    }
}

void LaumioLeds::setColumnColor(int column, uint8_t r, uint8_t g, uint8_t b)
{
    LaumioLeds::Led * pcolumn = nullptr;
    switch (column) {
    case 0:
        pcolumn = (LaumioLeds::Led *) & LaumioLeds::AColumn;
        break;
    case 1:
        pcolumn = (LaumioLeds::Led *) & LaumioLeds::BColumn;
        break;
    case 2:
        pcolumn = (LaumioLeds::Led *) & LaumioLeds::CColumn;
        break;
    case 3:
        pcolumn = (LaumioLeds::Led *) & LaumioLeds::DColumn;
        break;
    }

    if (pcolumn) {
        for (int i = 0; i < 3; i++) {
            strip.SetPixelColor(pcolumn[i], RgbColor(r, g, b));
        }
    }
}

void LaumioLeds::fillColor(const uint8_t & r,
                           const uint8_t & g, const uint8_t & b)
{
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, RgbColor(r, g, b));
    }
}

void LaumioLeds::show()
{
    strip.Show();
}

void LaumioLeds::animate(Animation a)
{
    switch (a) {
    case Animation::Clear:
        colorWipe(RgbColor(0, 0, 0), 50);
        break;
    case Animation::Hello:
        colorWipe(RgbColor(255, 0, 255), 50);
        colorWipe(RgbColor(0, 0, 0), 50);
        break;
    case Animation::Loading:
        colorWipe(RgbColor(255, 0, 0), 50);
        colorWipe(RgbColor(0, 0, 0), 50);
        break;
    case Animation::Happy:
        rainbowCycle(1);
        break;
    }
}

// Fill the dots one after the other with a color
void LaumioLeds::colorWipe(RgbColor c, uint8_t wait)
{
    uint16_t i;
    for (i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, c);
        strip.Show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void LaumioLeds::rainbowCycle(uint8_t wait)
{
    uint16_t i, j;
    for (j = 0; j < 256 * 5; j++) {     // 5 cycles of all colors on wheel
        for (i = 0; i < strip.PixelCount(); i++) {
            strip.SetPixelColor(i,
                                Wheel(((i * 256 /
                                        strip.PixelCount()) + j) & 255));
        }
        strip.Show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
RgbColor LaumioLeds::Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}
