#pragma once

#define RELAY_pin       A1  // Контакт реле
#define BTN_REL_pin     11  // Контакт кнопки, переключающей состояние реле
#define BTN_LCD_pin     10  // Контакт кнопки, включающей подсветку экрана
#define BTN_CONF_pin    9   // Контакт кнопки, включающей режим связи
#define CHECK_TIME      600000UL  // Время, раз в которое происходит проверка обстоятельств
#define DISPLAY_TIME    15000  // Время (мс), сколько горит дисплей после пробуждения
#define LS_pin          A6  // Контакт датчика освещённости
#define BUZZ_pin        8   // Контакт пьезозуммера
#define RTC_POW_pin     A3  // Контакт питания часов
#define RTC_GND_pin     A2  // Контакт заземления часов
#define LCD_LED_pin     12  // Контакт подсветки экрана
#define CONN_LED_pin    A0  // Контакт индикатора связи
#define NUM_OF_PARAMS   5   // Кол-во настроек
