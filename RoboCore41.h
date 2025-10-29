#ifndef ROBOCORE41_H
#define ROBOCORE41_H

#include <Arduino.h>
#include "pin_defines.h"

void boardInit();
void playStartupMelody();
void serialPassthrough(Stream &from, Stream &to);
void ledOn(uint8_t ledPin);
void ledOff(uint8_t ledPin);
void ledToggle(uint8_t ledPin);
void ledBlink(uint8_t ledPin, uint32_t intervalMs);

void buzzBeep(uint8_t buzzerPin, uint16_t durationMs = 100);

bool readButtonDebounced(uint8_t pin, uint32_t debounceTimeMs = 50);

int readAnalogAvg(uint8_t analogPin, uint8_t samples = 10);
float readAnalogCalibrated(uint8_t analogPin, float offset, float scale);

void spiInitDefault();

void printBoardInfo(Stream &serial);

bool checkTimeout(unsigned long &lastTime, unsigned long intervalMs);

void logMessage(Stream &serial, const char *msg);

#endif
