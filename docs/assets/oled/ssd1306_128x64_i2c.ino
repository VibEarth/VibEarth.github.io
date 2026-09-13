/**************************************************************************
  SSD1306 128x64 I2C OLED Example
  ESP32-C3 SuperMini

  I2C:
    SDA = GPIO6
    SCL = GPIO7
**************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ESP32-C3 SuperMini I2C pins
#define SDA_PIN 6
#define SCL_PIN 7

// OLED I2C address
#define SCREEN_ADDRESS 0x3C

// No dedicated reset pin
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

#define NUMFLAKES 10

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

static const unsigned char PROGMEM logo_bmp[] =
{
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

void setup() {
  Serial.begin(115200);

  // ESP32-C3 SuperMini
  // SDA = GPIO6
  // SCL = GPIO7
  Wire.begin(SDA_PIN, SCL_PIN);

  delay(500);

  Serial.println("SSD1306 128x64 I2C Test");
  Serial.println("SDA = GPIO6");
  Serial.println("SCL = GPIO7");
  Serial.println("Address = 0x3C");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");

    while (1) {
      delay(10);
    }
  }

  Serial.println("SSD1306 detected!");

  // Show Adafruit splash screen
  display.display();
  delay(2000);

  // Clear display
  display.clearDisplay();

  // Draw a single pixel
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);

  // Graphics tests
  testdrawline();
  testdrawrect();
  testfillrect();
  testdrawcircle();
  testfillcircle();
  testdrawroundrect();
  testfillroundrect();
  testdrawtriangle();
  testfilltriangle();
  testdrawchar();
  testdrawstyles();
  testscrolltext();
  testdrawbitmap();

  // Invert and restore display
  display.invertDisplay(true);
  delay(1000);

  display.invertDisplay(false);
  delay(1000);

  // Animation
  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);
}

void loop() {
}


// --------------------------------------------------
// Draw lines
// --------------------------------------------------

void testdrawline() {
  int16_t i;

  display.clearDisplay();

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(
      0,
      0,
      i,
      display.height() - 1,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(
      0,
      0,
      display.width() - 1,
      i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(250);

  display.clearDisplay();

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(
      0,
      display.height() - 1,
      i,
      0,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(
      0,
      display.height() - 1,
      display.width() - 1,
      i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(250);

  display.clearDisplay();

  for (i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(
      display.width() - 1,
      display.height() - 1,
      i,
      0,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  for (i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(
      display.width() - 1,
      display.height() - 1,
      0,
      i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(250);

  display.clearDisplay();

  for (i = 0; i < display.height(); i += 4) {
    display.drawLine(
      display.width() - 1,
      0,
      0,
      i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  for (i = 0; i < display.width(); i += 4) {
    display.drawLine(
      display.width() - 1,
      0,
      i,
      display.height() - 1,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Rectangles
// --------------------------------------------------

void testdrawrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(
      i,
      i,
      display.width() - 2 * i,
      display.height() - 2 * i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Filled rectangles
// --------------------------------------------------

void testfillrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    display.fillRect(
      i,
      i,
      display.width() - i * 2,
      display.height() - i * 2,
      SSD1306_INVERSE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Circles
// --------------------------------------------------

void testdrawcircle(void) {
  display.clearDisplay();

  for (
    int16_t i = 0;
    i < max(display.width(), display.height()) / 2;
    i += 2
  ) {
    display.drawCircle(
      display.width() / 2,
      display.height() / 2,
      i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Filled circles
// --------------------------------------------------

void testfillcircle(void) {
  display.clearDisplay();

  for (
    int16_t i = max(display.width(), display.height()) / 2;
    i > 0;
    i -= 3
  ) {
    display.fillCircle(
      display.width() / 2,
      display.height() / 2,
      i,
      SSD1306_INVERSE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Rounded rectangles
// --------------------------------------------------

void testdrawroundrect(void) {
  display.clearDisplay();

  for (
    int16_t i = 0;
    i < display.height() / 2 - 2;
    i += 2
  ) {
    display.drawRoundRect(
      i,
      i,
      display.width() - 2 * i,
      display.height() - 2 * i,
      display.height() / 4,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Filled rounded rectangles
// --------------------------------------------------

void testfillroundrect(void) {
  display.clearDisplay();

  for (
    int16_t i = 0;
    i < display.height() / 2 - 2;
    i += 2
  ) {
    display.fillRoundRect(
      i,
      i,
      display.width() - 2 * i,
      display.height() - 2 * i,
      display.height() / 4,
      SSD1306_INVERSE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Triangles
// --------------------------------------------------

void testdrawtriangle(void) {
  display.clearDisplay();

  for (
    int16_t i = 0;
    i < max(display.width(), display.height()) / 2;
    i += 5
  ) {
    display.drawTriangle(
      display.width() / 2,
      display.height() / 2 - i,
      display.width() / 2 - i,
      display.height() / 2 + i,
      display.width() / 2 + i,
      display.height() / 2 + i,
      SSD1306_WHITE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Filled triangles
// --------------------------------------------------

void testfilltriangle(void) {
  display.clearDisplay();

  for (
    int16_t i = max(display.width(), display.height()) / 2;
    i > 0;
    i -= 5
  ) {
    display.fillTriangle(
      display.width() / 2,
      display.height() / 2 - i,
      display.width() / 2 - i,
      display.height() / 2 + i,
      display.width() / 2 + i,
      display.height() / 2 + i,
      SSD1306_INVERSE
    );

    display.display();
    delay(1);
  }

  delay(2000);
}


// --------------------------------------------------
// Characters
// --------------------------------------------------

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);

  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') {
      display.write(' ');
    } else {
      display.write(i);
    }
  }

  display.display();
  delay(2000);
}


// --------------------------------------------------
// Text styles
// --------------------------------------------------

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(F("Hello, world!"));

  display.setTextColor(
    SSD1306_BLACK,
    SSD1306_WHITE
  );

  display.println(3.141592);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.print(F("0x"));
  display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}


// --------------------------------------------------
// Scrolling text
// --------------------------------------------------

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);

  display.println(F("scroll"));
  display.display();

  delay(100);

  display.startscrollright(0x00, 0x0F);
  delay(2000);

  display.stopscroll();
  delay(1000);

  display.startscrollleft(0x00, 0x0F);
  delay(2000);

  display.stopscroll();
  delay(1000);

  display.startscrolldiagright(0x00, 0x07);
  delay(2000);

  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);

  display.stopscroll();
  delay(1000);
}


// --------------------------------------------------
// Bitmap
// --------------------------------------------------

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width() - LOGO_WIDTH) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp,
    LOGO_WIDTH,
    LOGO_HEIGHT,
    1
  );

  display.display();
  delay(1000);
}


// --------------------------------------------------
// Animation
// --------------------------------------------------

#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testanimate(
  const uint8_t *bitmap,
  uint8_t w,
  uint8_t h
) {
  int8_t f;
  int8_t icons[NUMFLAKES][3];

  // Initialize snowflake positions
  for (f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS] = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);

    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);

    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);

    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for (;;) {
    display.clearDisplay();

    // Draw each snowflake
    for (f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(
        icons[f][XPOS],
        icons[f][YPOS],
        bitmap,
        w,
        h,
        SSD1306_WHITE
      );
    }

    display.display();

    delay(200);

    // Update coordinates
    for (f = 0; f < NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];

      // If snowflake is off the bottom
      if (icons[f][YPOS] >= display.height()) {
        icons[f][XPOS] = random(
          1 - LOGO_WIDTH,
          display.width()
        );

        icons[f][YPOS] = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}