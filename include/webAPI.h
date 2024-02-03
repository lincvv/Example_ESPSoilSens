#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


typedef struct Setting 
{
    int32_t timeToSleep;

}SensSetting;

// extern  SensSetting settings;
void POSTDataMeasure(float temp, uint16_t capread, char mac[]);
bool GetSettings(char mac[]);
uint32_t getNumberSens();

