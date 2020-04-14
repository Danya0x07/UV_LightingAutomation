#include "LightingSession.h"
#include <EEPROM.h>

uint8_t LightingSession::nextId = 0;

LightingSession::LightingSession() : startTime(0), endTime(0)
{
    lightThreshold = 40;  // default stubby value
    isActive = false;
    id = nextId++;
}

void LightingSession::loadFromEeprom(uint16_t address)
{
    uint8_t active = EEPROM.read(address);
    uint8_t lt = EEPROM.read(address + 1);
    uint8_t sh = EEPROM.read(address + 2);
    uint8_t sm = EEPROM.read(address + 3);
    uint8_t eh = EEPROM.read(address + 4);
    uint8_t em = EEPROM.read(address + 5);

    if (sh > 23 || eh > 23 || sm > 59 || em > 59 || 
            lt > 100 || ((active & ~0x01) != 0)) {
        return;
    }

    isActive = active;
    lightThreshold = lt;
    startTime = DateTime(0, 0, 0, sh, sm, 0);
    endTime = DateTime(0, 0, 0, eh, em, 0);
}

void LightingSession::loadFromEeprom()
{
    loadFromEeprom(id * getActualEepromPayloadSize());
}

void LightingSession::saveToEeprom(uint16_t address)
{
    EEPROM.update(address + 0, isActive);
    EEPROM.update(address + 1, lightThreshold);
    EEPROM.update(address + 2, startTime.hour());
    EEPROM.update(address + 3, startTime.minute());
    EEPROM.update(address + 4, endTime.hour());
    EEPROM.update(address + 5, endTime.minute());
}

void LightingSession::saveToEeprom()
{
    saveToEeprom(id * getActualEepromPayloadSize());
}

bool LightingSession::hasToBeUnderway(const DateTime& currentTime, uint8_t lightLevel)
{
    DateTime relativeTime = DateTime(0, 0, 0, currentTime.hour(), currentTime.minute(), 0);
    return isActive && (lightLevel <= lightThreshold) && 
           (relativeTime >= startTime) && (relativeTime < endTime);
}

void LightingSession::setLightThreshold(uint8_t threshold)
{
    lightThreshold = min(threshold, 100);
}

void LightingSession::setStartTime(uint8_t hour, uint8_t minute)
{
    hour = min(hour, 23);
    minute = min(minute, 59);
    startTime = DateTime(0, 0, 0, hour, minute, 0);
}

void LightingSession::setEndTime(uint8_t hour, uint8_t minute)
{
    hour = min(hour, 23);
    minute = min(minute, 59);
    endTime = DateTime(0, 0, 0, hour, minute, 0);
}

void LightingSession::getStartTime(uint8_t* hour, uint8_t* minute)
{
    *hour = startTime.hour();
    *minute = startTime.minute();
}

void LightingSession::getEndTime(uint8_t* hour, uint8_t* minute)
{
    *hour = endTime.hour();
    *minute = endTime.minute();
}

bool LightingSession::operator==(const LightingSession& other)
{
    return this->isActive == other.isActive && 
           this->lightThreshold == other.lightThreshold &&
           this->startTime == other.startTime &&
           this->endTime == other.endTime;
}

bool LightingSession::operator!=(const LightingSession& other)
{
    return !(*this == other);
}