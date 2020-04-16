#pragma once

#include <Arduino.h>
#include <unity.h>

#include <HardwareManager.h>
#include <LightingSession.h>
#include <UI.h>

extern HardwareManager hardware;
extern LightingSession morningSession, eveningSession;
extern UserInterface ui;

void testRelay();
void testButtons();
void testClock();

void testLightingSession();

void testMenuTransitions();
void testMainMenuUI();
void testSessionConfiguringUI();
void testClockConfiguringUI();
