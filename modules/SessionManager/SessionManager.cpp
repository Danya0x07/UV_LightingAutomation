#include "SessionManager.h"

#define STARTADDR   0

SessionManager::SessionManager()
    : morning(LightingSession::ON_LIGHT_INCREASING),
    evening(LightingSession::ON_LIGHT_DECREASING),
    selected(&morning), performedToday(0)
{
    morning.loadFromEeprom(STARTADDR + 0 * LightingSession::getActualEepromPayloadSize());
    evening.loadFromEeprom(STARTADDR + 1 * LightingSession::getActualEepromPayloadSize());
}

uint8_t SessionManager::check(const DateTime& currentTime, uint8_t lightLevel)
{
    uint8_t underway = 0;

    if (morning.hasToBeUnderway(currentTime, lightLevel)) {
        underway |= SESSION_MORNING;
    }
    if (evening.hasToBeUnderway(currentTime, lightLevel)) {
        underway |= SESSION_EVENING;
    }

    performedToday |= underway;
    if (currentTime.hour() == 0 && currentTime.minute() == 0) {
        /* В полночь обнуляем прошедшие сеансы. */
        performedToday = 0;
    }

    return underway;
}

uint8_t SessionManager::getPerformedToday()
{
    return performedToday;
}

void SessionManager::select(LightingSession* session)
{
    if (session != nullptr)
        selected = session;
}

void SessionManager::save()
{
#ifndef UNIT_TEST  // Тесты не должны перетирать сохранённые ранее настройки
    morning.saveToEeprom(STARTADDR + 0 * LightingSession::getActualEepromPayloadSize());
    evening.saveToEeprom(STARTADDR + 1 * LightingSession::getActualEepromPayloadSize());
#endif
}
