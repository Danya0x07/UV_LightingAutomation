#pragma once

#include <Arduino.h>
#include <RTClib.h>

class LightingSession
{
protected:
    bool isActive;
    uint8_t lightThreshold;
    DateTime startTime;
    DateTime endTime;

public:
    LightingSession();
    void loadFromEeprom(uint16_t address);
    void saveToEeprom(uint16_t address);
    bool hasToBeUnderway(const DateTime& currentTime, uint8_t lightLevel);
    void setActive(bool active) {isActive = active;}
};