#pragma once

#include <Arduino.h>
#include <unity.h>

#include <HardwareManager.h>
#include <LightingSession.h>
#include <UI.h>

extern HardwareManager hardware;
extern SessionManager sessions;
extern UserInterface ui;

void setUpTestSession();

void testRelay();
void testButtons();
void testClock();

void testSessionUnderwayDetection();
void testSessionSavingLoading();
void testSessionManager();

void testMenuTransitions();
void testMainMenuUI();
void testSessionConfiguringUI();
void testClockConfiguringUI();
