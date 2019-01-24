#include "LaumioConfig.h"
#include <EEPROM.h>

void LaumioConfig::readFromEEPROM() {
    unsigned int totalsz = 3;
    for (auto * param : paramList)
        totalsz += param->size();
    EEPROM.begin(totalsz);

    if (EEPROM.read(0) != VERSION) {
        EEPROM.end();
        return;
    }

    unsigned int regsz = EEPROM.read(1) | (EEPROM.read(2) << 8);
    unsigned int pos = 3;
    for (auto * param : paramList) {
        unsigned int sz = param->size();
        if (pos + sz > regsz)
            break; // Keep default data for other parameters
        char * buffer = param->buffer();
        for (int i = 0; i < sz; ++i) {
            buffer[i] = EEPROM.read(pos + i);
        }
        pos += sz;
    }

    EEPROM.end();
}

void LaumioConfig::saveToEEPROM() {
    unsigned int totalsz = 3;
    for (auto * param : paramList)
        totalsz += param->size();
    EEPROM.begin(totalsz);

    unsigned int pos = 3;
    EEPROM.write(0, VERSION);
    EEPROM.write(1, totalsz & 0xff);
    EEPROM.write(2, (totalsz >> 8) & 0xff);
    for (auto * param : paramList) {
        unsigned int sz = param->size();
        char * buffer = param->buffer();
        for (int i = 0; i < sz; ++i) {
            EEPROM.write(pos + i, buffer[i]);
        }
        pos += sz;
    }

    EEPROM.commit();
    EEPROM.end();
}
