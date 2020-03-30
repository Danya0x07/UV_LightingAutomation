#include "data.h"
#include "macro.h"
#include "Relay.h"
#include <RTClib.h>
#include <LiquidCrystal.h>

extern RTC_DS1307 RTClock;
extern DateTime dt;
extern LiquidCrystal lcd;
extern Relay relay;
extern bool was_morning_light;
extern bool was_evening_light;

uint8_t get_light_value()
{
    uint16_t val = analogRead(LS_pin);
    val = map(val, 0, 1023, 0, 100);
    return uint8_t(val);
}

String get_current_time()
{
    dt = RTClock.now();
    String hour = String(dt.hour(), DEC) + ':';
    String minute = String(dt.minute(), DEC) + ' ';
    return hour + minute;
}

void update_display()
{
    lcd.display();
    lcd.setCursor(7, 0);
    lcd.print(get_current_time());
    lcd.setCursor(15, 0);
    lcd.print(relay.getState() ? 'R' : ' ');
    lcd.setCursor(7, 1);
    lcd.print(get_light_value());
    lcd.print("%  ");
    lcd.setCursor(14, 1);
    lcd.print(was_morning_light ? 'M' : ' ');
    lcd.setCursor(15, 1);
    lcd.print(was_evening_light ? 'E' : ' ');
}

