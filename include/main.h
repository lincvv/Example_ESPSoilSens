#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "Adafruit_seesaw.h"
#include "string.h"
#include "webAPI.h"
#include "WIFIUtil.h"

// I2C pin 21, 22
// Led pins 25, 26, 27
// External pins 5, 18, 19, 23, 16, 17

#define A_FLAG_IS_NUMBER_SENS 0
#define A_NUMBER_SENS 1
#define PIN_BTN_RST_SSID 4
#define FLAG_IS_NUMBER_SENS 255
#define I2C_ADDRESS 0x36

#define uS_TO_S_FACTOR 1000000L
#define uS_TO_M_FACTOR 60000000L
// uint64_t  timeToSleep = 5400000000L; // 90 min
// uint64_t timeToSleep = 60 * uS_TO_S_FACTOR; // 10 sec