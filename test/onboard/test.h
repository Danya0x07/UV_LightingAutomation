#pragma once

#include <Arduino.h>
#include <unity.h>

#include <HardwareManager.h>
#include <LightingSession.h>
#include <UI.h>

extern HardwareManager hardware;

void testRelay();
void testButtons();
void testClock();

void testLightingSession();

void testMainMenuUI();
void testSessionConfiguringUI();
void testClockConfiguringUI();
