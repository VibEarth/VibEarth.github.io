#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ============================================================
// ESP32-C3 SuperMini
// I2C
// ============================================================

#define SDA_PIN 6
#define SCL_PIN 7


// ============================================================
// I2C Addresses
// ============================================================

#define BME280_ADDRESS 0x76
#define OLED_ADDRESS   0x3C


// ============================================================
// BME280
// ============================================================

Adafruit_BME280 bme;

#define SEALEVELPRESSURE_HPA 1013.25


// ============================================================
// OLED SSD1306 128x64
// ============================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// ============================================================
// Update interval
// ============================================================

const unsigned long UPDATE_INTERVAL = 1000;

unsigned long lastUpdate = 0;


// ============================================================
// Update counter
// ============================================================

unsigned long updateCount = 0;


// ============================================================
// Sensor values
// ============================================================

float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float altitude = 0.0;


// ============================================================
// Setup
// ============================================================

void setup() {

  // ----------------------------------------------------------
  // Serial
  // ----------------------------------------------------------

  Serial.begin(115200);


  // ----------------------------------------------------------
  // I2C initialization
  // ----------------------------------------------------------

  Wire.begin(SDA_PIN, SCL_PIN);

  // 400 kHz Fast Mode
  Wire.setClock(400000);


  // ----------------------------------------------------------
  // BME280
  // ----------------------------------------------------------

  bme.begin(BME280_ADDRESS, &Wire);


  // ----------------------------------------------------------
  // OLED
  // ----------------------------------------------------------

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    while (true) {
      delay(1000);
    }
  }


  // ----------------------------------------------------------
  // Startup screen
  // ----------------------------------------------------------

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("ESP32-C3 SENSOR");

  display.setCursor(0, 14);
  display.println("BME280 + SSD1306");

  display.setCursor(0, 30);
  display.println("SDA : GPIO6");

  display.setCursor(0, 42);
  display.println("SCL : GPIO7");

  display.setCursor(0, 54);
  display.println("I2C : 400 kHz");

  display.display();

  delay(1500);


  // ----------------------------------------------------------
  // First sensor update
  // ----------------------------------------------------------

  updateSensorAndDisplay();

  lastUpdate = millis();
}


// ============================================================
// Main loop
// ============================================================

void loop() {

  unsigned long currentMillis = millis();

  // ----------------------------------------------------------
  // Update every 1 second
  // ----------------------------------------------------------

  if (currentMillis - lastUpdate >= UPDATE_INTERVAL) {

    lastUpdate += UPDATE_INTERVAL;

    updateSensorAndDisplay();
  }
}


// ============================================================
// Read BME280 and update OLED
// ============================================================

void updateSensorAndDisplay() {

  // ----------------------------------------------------------
  // Update counter
  // ----------------------------------------------------------

  updateCount++;


  // ----------------------------------------------------------
  // Read BME280
  // ----------------------------------------------------------

  temperature = bme.readTemperature();

  humidity = bme.readHumidity();

  pressure = bme.readPressure() / 100.0F;

  altitude = bme.readAltitude(
    SEALEVELPRESSURE_HPA
  );


  // ==========================================================
  // OLED
  // ==========================================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // ----------------------------------------------------------
  // Temperature
  // ----------------------------------------------------------

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.print("TEMP ");

  display.setTextSize(2);

  display.print(temperature, 2);

  display.setTextSize(1);

  display.println(" C");


  // ----------------------------------------------------------
  // Humidity
  // ----------------------------------------------------------

  display.setCursor(0, 16);

  display.print("HUM  ");

  display.setTextSize(2);

  display.print(humidity, 2);

  display.setTextSize(1);

  display.println(" %");


  // ----------------------------------------------------------
  // Pressure
  // ----------------------------------------------------------

  display.setCursor(0, 32);

  display.print("PRES ");

  display.setTextSize(2);

  display.print(pressure, 2);

  display.setTextSize(1);

  display.println(" hPa");


  // ----------------------------------------------------------
  // Altitude + Update counter
  // ----------------------------------------------------------

  display.setTextSize(1);

  display.setCursor(0, 50);

  display.print("ALT  ");

  display.print(altitude, 1);

  display.print(" m");

  display.setCursor(82, 50);

  display.print("t:");

  display.print(updateCount);

  display.print("s");


  // ----------------------------------------------------------
  // Send framebuffer to OLED
  // ----------------------------------------------------------

  display.display();


  // ==========================================================
  // Serial Monitor / Serial Plotter
  // ==========================================================
  //
  // Plotter-compatible format:
  //
  // TEMP:24.31    HUM:52.40    PRES:1012.83    ALT:8.4
  //
  // No units or extra text are included.
  //
  // This allows Arduino IDE Serial Plotter to recognize
  // four independent numeric series.
  //
  // ==========================================================

  if (Serial.availableForWrite() >= 80) {

    Serial.print("TEMP:");
    Serial.print(temperature, 2);

    Serial.print("\tHUM:");
    Serial.print(humidity, 2);

    Serial.print("\tPRES:");
    Serial.print(pressure, 2);

    Serial.print("\tALT:");
    Serial.println(altitude, 1);
  }
}