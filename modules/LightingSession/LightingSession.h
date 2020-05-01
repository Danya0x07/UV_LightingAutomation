#pragma once

#include <Arduino.h>
#include <RTClib.h>

/**
 * Представление сеанса УФ досветки.
 * Определяется начальным и конечным временами,
 * пороговым значением освещённости и текущим статусом
 * активности.
 */
class LightingSession
{
public:
    /**
     * Переход через порог освещённости, означающий начало сеанса.
     * Может быть или "снизу вверх", или "сверху вниз".
     * (Вечером освещённость уменьшается, утром - увеличивается.)
     */
    enum LightTrigger: uint8_t {
        ON_INCREASING,
        ON_DECREASING
    };

    explicit LightingSession(LightTrigger);

    void loadFromEeprom(uint16_t address);
    void saveToEeprom(uint16_t address);
    bool hasToBeUnderway(const DateTime& currentTime, uint8_t lightLevel);

    void setActive(bool active) { isActive = active; }
    void setLightThreshold(uint8_t);
    void setStartTime(uint8_t hour, uint8_t minute);
    void setEndTime(uint8_t hour, uint8_t minute);

    bool getActive() { return isActive; }
    uint8_t getLightThreshold() { return lightThreshold; }
    DateTime getStartTime() { return startTime; }
    DateTime getEndTime() { return endTime; }
    void getStartTime(uint8_t* hour, uint8_t* minute);
    void getEndTime(uint8_t* hour, uint8_t* minute);

    bool operator==(const LightingSession&);
    bool operator!=(const LightingSession&);

    static uint8_t getActualEepromPayloadSize() { return 6; }

protected:
    const LightTrigger trigger;
    bool thresholdReached;

    /* Часть, которая сохраняется в EEPROM. */
    bool isActive;
    uint8_t lightThreshold;
    DateTime startTime;
    DateTime endTime;
};
