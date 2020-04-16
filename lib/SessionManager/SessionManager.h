#pragma once

#include <Arduino.h>

#include <LightingSession.h>

/** Фасад для сеансов досветки. */
class SessionManager
{
private:
    LightingSession* selected;
    uint8_t performedToday;

public:
    LightingSession morning;
    LightingSession evening;

    enum Session : uint8_t {
        SESSION_MORNING = 1 << 0,
        SESSION_EVENING = 1 << 1,

        /**
         * Значение, используемое для проверок того, что идёт/прошёл
         * хотя бы один сеанс.
         */
        SESSION_ANY = SESSION_MORNING | SESSION_EVENING
    };

    SessionManager();

    /**
     * Обновляет данные о сеансах, прошедших в текущие сутки.
     * Возвращает битовую маску из перечисления Session, содержащую
     * сеансы досветки, которые должны выполняться в текущее время (currentTime)
     * при текущем уровне освещённости (lightLevel).
     */
    uint8_t check(const DateTime& currentTime, uint8_t lightLevel);

    /**
     * Возвращает битовую маску из перечисление Session, содержащую
     * сеансы досветки, которые были в текущих сутках.
     */
    uint8_t getPerformedToday();

    /** Позволяет отметить сеанс (не nullptr) как выбранный. */
    void select(LightingSession*);

    /** Возвращает выбранный ранее сеанс. По умолчанию, morning. */
    LightingSession* getSelected() { return selected; }

    /** Сохраняет сеансы в энергонезависимую память. */
    void save();
};
