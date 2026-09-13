#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SDA 4
#define BME_SCL 5
#define BME280_ADDR 0x77  // 안되면 0x76 시도

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("BME280 테스트 시작");

  Wire.begin(BME_SDA, BME_SCL);

  if (!bme.begin(BME280_ADDR)) {
    Serial.println("BME280을 찾을 수 없습니다. 배선과 I2C 주소를 확인하세요.");
    Serial.println("SDO를 GND에 연결하면 주소가 0x76으로 바뀝니다.");
    while (1);
  }

  Serial.println("BME280 연결 성공!");
}

void loop() {
  Serial.println("----------");
  Serial.print("온도: ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("습도: ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.print("기압: ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("고도(추정): ");
  Serial.print(bme.readAltitude(1013.25));
  Serial.println(" m");

  delay(2000);
}