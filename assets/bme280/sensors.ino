#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>

// MJKDZ BME280 — GPIO6, GPIO7
#define MJKDZ_SDA 6
#define MJKDZ_SCL 7
#define MJKDZ_ADDR 0x77

// Adafruit BME280 — GPIO4, GPIO5
#define ADAFRUIT_SDA 4
#define ADAFRUIT_SCL 5
#define ADAFRUIT_ADDR 0x77

// DHT11
#define DHT_PIN 3
#define DHT_TYPE DHT11

Adafruit_BME280 bme;
DHT dht(DHT_PIN, DHT_TYPE);

float t1, h1, p1;  // MJKDZ
float t2, h2, p2;  // Adafruit
float t3, h3;      // DHT11 (기압 없음)

void readMJKDZ() {
  Wire.begin(MJKDZ_SDA, MJKDZ_SCL);
  delay(100);
  if (!bme.begin(MJKDZ_ADDR, &Wire)) {
    Serial.println("[MJKDZ] 연결 실패!");
    return;
  }
  t1 = bme.readTemperature();
  h1 = bme.readHumidity();
  p1 = bme.readPressure() / 100.0F;
}

void readAdafruit() {
  Wire.begin(ADAFRUIT_SDA, ADAFRUIT_SCL);
  delay(100);
  if (!bme.begin(ADAFRUIT_ADDR, &Wire)) {
    Serial.println("[Adafruit] 연결 실패!");
    return;
  }
  t2 = bme.readTemperature();
  h2 = bme.readHumidity();
  p2 = bme.readPressure() / 100.0F;
}

void readDHT11() {
  t3 = dht.readTemperature();
  h3 = dht.readHumidity();
  if (isnan(t3) || isnan(h3)) {
    Serial.println("[DHT11] 읽기 실패!");
    t3 = 0; h3 = 0;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  Serial.println("3종 센서 비교 시작");
}

void loop() {
  readMJKDZ();
  delay(200);
  readAdafruit();
  delay(200);
  readDHT11();

  Serial.println("=============================================");
  Serial.println("항목        MJKDZ      Adafruit   DHT11");
  Serial.println("---------------------------------------------");
  Serial.printf("온도(°C)   %6.2f     %6.2f     %6.2f\n", t1, t2, t3);
  Serial.printf("습도(%%)    %6.2f     %6.2f     %6.2f\n", h1, h2, h3);
  Serial.printf("기압(hPa)  %7.2f    %7.2f    (없음)\n", p1, p2);
  Serial.println();
  Serial.println("--- 차이 (MJKDZ 기준) ---");
  Serial.printf("온도 Adafruit: %+.2f / DHT11: %+.2f\n", t1-t2, t1-t3);
  Serial.printf("습도 Adafruit: %+.2f / DHT11: %+.2f\n", h1-h2, h1-h3);

  delay(2000);
}