#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "web_ui.h"

// ===================== 사용자 설정 =====================
#define BME_SDA_PIN   6
#define BME_SCL_PIN   7
#define BME280_I2C_ADDR 0x77

const char* AP_SSID = "C3-Weather";
const char* AP_PASS = "12345678";

static const uint32_t SAMPLE_INTERVAL_MS = 1000;

// ===================== 전역 객체 =====================
Adafruit_BME280 bme;

// ===================== 상태 변수 =====================
bool gBmeOk = false;
uint32_t gLastSampleMs = 0;
WebSensorData gWebData = {0};

// ===================== setup =====================
void setup() {
  Serial.begin(115200);
  unsigned long t = millis();
  while (!Serial && millis() - t < 2000);  // 최대 2초 대기 후 진행

  Serial.println();
  Serial.println("========================================");
  Serial.println("ESP32-C3 SuperMini  Weather Station");
  Serial.println("BME280 + WiFi Web UI");
  Serial.println("========================================");

  // I2C
  Wire.begin(BME_SDA_PIN, BME_SCL_PIN);
  Wire.setClock(100000);

  // BME280
  if (!bme.begin(BME280_I2C_ADDR)) {
    Serial.println("[BME280] not found - check wiring");
    gBmeOk = false;
  } else {
    bme.setSampling(
      Adafruit_BME280::MODE_FORCED,
      Adafruit_BME280::SAMPLING_X4,  // 온도: 4회 평균
      Adafruit_BME280::SAMPLING_X4,  // 기압: 4회 평균
      Adafruit_BME280::SAMPLING_X4,  // 습도: 4회 평균
      Adafruit_BME280::FILTER_OFF,
      Adafruit_BME280::STANDBY_MS_1000
    );
    gBmeOk = true;
    Serial.println("[BME280] ready");
  }

  // 웹서버 시작
  gWebData.bmeOk = gBmeOk;
  webUiBegin(AP_SSID, AP_PASS);
  // 첫 실측 전 webUiUpdate 호출하지 않음 → 히스토리에 0 삽입 방지

  Serial.println("----------------------------------------");
  Serial.print("AP SSID : "); Serial.println(AP_SSID);
  Serial.print("Password: "); Serial.println(AP_PASS);
  Serial.print("Web UI  : http://"); Serial.println(webUiGetApIp());
  Serial.println("----------------------------------------");

  gLastSampleMs = millis();
}

// ===================== loop =====================
void loop() {
  webUiLoop();

  if (millis() - gLastSampleMs < SAMPLE_INTERVAL_MS) {
    delay(10);
    return;
  }
  gLastSampleMs = millis();

  float temp     = NAN;
  float humidity = NAN;
  float pressure = NAN;
  float altitude = NAN;

  if (gBmeOk) {
    bme.takeForcedMeasurement();
    temp     = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
    altitude = bme.readAltitude(1013.25);
  }

  Serial.print("-------------- SAMPLE #");
  Serial.print(gWebData.readCount + 1);
  Serial.println(" ------------------");
  if (gBmeOk) {
    Serial.print("Temp     : "); Serial.println(temp,     1);
    Serial.print("Humidity : "); Serial.println(humidity, 1);
    Serial.print("Pressure : "); Serial.println(pressure, 1);
    Serial.print("Altitude : "); Serial.println(altitude, 1);
  } else {
    Serial.println("BME280 unavailable");
  }

  gWebData.temp         = temp;
  gWebData.humidity     = humidity;
  gWebData.pressure     = pressure;
  gWebData.altitude     = altitude;
  gWebData.bmeOk        = gBmeOk;
  gWebData.readCount++;
  gWebData.lastUpdateMs = millis();
  webUiUpdate(gWebData);
}
