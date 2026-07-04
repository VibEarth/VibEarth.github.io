#ifndef WEB_UI_H
#define WEB_UI_H

#include <Arduino.h>

#define HISTORY_SIZE 1200   // 0.5초 × 1200 = 10분

struct WebSensorData {
  float temp;
  float humidity;
  float pressure;
  float altitude;

  bool bmeOk;

  uint32_t readCount;
  uint32_t lastUpdateMs;
};

void webUiBegin(const char* apSsid, const char* apPass);
void webUiLoop();
void webUiUpdate(const WebSensorData& data);
String webUiGetApIp();

#endif