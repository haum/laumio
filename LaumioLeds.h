#ifndef LAUMIOLEDS_H
#define LAUMIOLEDS_H

#include <Adafruit_NeoPixel.h>

class LaumioLeds {
  public:
    enum Animation {
        Clear,
        Hello,
        Loading,
        Happy,
    };
    LaumioLeds(uint16_t n, uint8_t p);
    void begin();
    void animate(Animation a);

    void colorWipe(uint32_t c, uint8_t wait);
    void rainbowCycle(uint8_t wait);
    uint32_t Wheel(byte WheelPos);
  private:
     Adafruit_NeoPixel strip;
};

#endif                          // LAUMIOLEDS_H
