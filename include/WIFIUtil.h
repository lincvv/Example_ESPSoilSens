#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

#define LED_STATUS_WIFI_STA 27
#define LED_ERROR 26
#define LED_STATUS_CONECT_OK 25
#define A_FLAG_IS_CONFIG 4
#define FLAG_IS_NOT_CONFIG 0
#define A_LENGHT_SSID 2
#define A_LENGHT_PASS 3
#define FLAG_IS_CONFIG 255
#define WAITE_SSID_CONNECT 30


extern uint8_t start_addr_ssid;
extern uint8_t start_addr_pass;
extern uint8_t eepromSize;

void connectWIFI_STA();
void connectSSID();



