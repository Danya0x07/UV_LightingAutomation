#pragma once

#include <Arduino.h>

#include <LightingSession.h>

/** Фасад для сеансов досветки. */
class SessionManager
{
private:
    LightingSession* selected;

public:
    LightingSession morning;
    LightingSession evening;

    enum Session : uint8_t {
        SESSION_MORNING = 1 << 0,
        SESSION_EVENING = 1 << 1,
        SESSION_ANY = SESSION_MORNING | SESSION_EVENING
    };

    SessionManager();

    uint8_t getUnderway(const DateTime& currentTime, uint8_t lightLevel);
    uint8_t getPerformedToday();

    void select(LightingSession*);
    LightingSession* getSelected() { return selected; }
};
