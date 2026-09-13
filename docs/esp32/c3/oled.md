# ESP32 Project - C3 SuperMini - 0.96" OLED Display

## 왜 OLED인가? — TFT LCD와의 비교

BME280 센서의 측정값을 출력할 디스플레이를 선택할 때, 가장 흔히 사용되는 **16×2 LCD**와 **0.96" 128x64 OLED**를 비교해보겠습니다.

![[oled_display.png]]

<small>Table 1. 16×2 LCD vs 0.96" 128x64 OLED 비교</small>

|      항목       |     16×2 LCD      |       0.96" OLED        |
| :-----------: | :---------------: | :---------------------: |
|     해상도      | 16자 × 2줄 (문자만) | 128×64 픽셀 (그래픽 가능) |
|    통신 방식    |  병렬(8핀) 또는 I2C  |      **I2C (2핀)**       |
|     배선 수     |      최소 6~8핀      | **VCC, GND, SDA, SCL — 4핀** |
|      크기       |     80×36mm      |      **25×27mm**        |
|    백라이트     |  필요 (전력 소모↑)   |  **자체 발광 (백라이트 불필요)**  |
|      가격       |     약 2,000원~     |       약 2,500원~        |
|    가독성      |        보통         |   **명암비 높아 선명**    |

OLED는 픽셀 하나하나가 스스로 빛을 내기 때문에 백라이트가 필요 없고, **ESP32-C3 SuperMini, BME280 센서와 함께 손바닥 만한 400홀 브레드보드에 장착하기 딱 좋습니다.**

> 기상관측드론에 장착할 센서 패키지는 작고 가벼워야 합니다. OLED는 LCD 대비 절반 이하의 크기로 동일한(혹은 더 풍부한) 정보를 표시할 수 있어, 드론 탑재 기상관측기의 디스플레이로 최적입니다.

---

## I2C의 장점 — 배선 추가가 이렇게 단순해도 되나?

BME280에 이어 OLED도 **I2C 통신**을 사용합니다. I2C의 핵심은 **하나의 버스(SDA/SCL 2선)에 여러 장치를 동시에 연결**할 수 있다는 점입니다.

![[C3cirkit.png]]

BME280과 OLED를 **같은 SDA/SCL 핀에 병렬로 연결**하면, 나중에 두 센서값을 OLED에 바로 출력할 수 있습니다. 배선 추가 없이 소프트웨어만 바꾸면 됩니다.

---

## 배선 연결 (wiring)

<small>Table 2. OLED → ESP32-C3 SuperMini 핀 연결</small>

| OLED 핀 | ESP32-C3 핀 |  색상  |
| :-----: | :---------: | :---------: |
|   VCC   |    3.3V     |     빨강     |
|   GND   |     GND     |     검정     |
|   SDA   |   GPIO6    |     주황     |
|   SCL   |   GPIO7    |     파랑     |

> ⚠️ ESP32-C3의 I2C 핀을 GPIO6/7로 BME280과 동일하게 연결합니다.

![[oled-wiring.png]]
---

## 라이브러리 설치

**Tools → Manage Libraries → `SSD1306` 검색 → Adafruit SSD1306 → Install**

의존 라이브러리 설치 팝업이 뜨면 **Install All** 선택합니다.

---

## 예제 코드로 동작 확인

**File → Examples → Adafruit SSD1306 → `ssd1306_128x64_i2c`** 열기

수정 없이 업로드하면 Adafruit 로고와 도형 애니메이션이 출력됩니다.

---

<div style="display: flex; justify-content: space-between; margin-top: 3rem; border-top: 1px solid var(--md-default-fg-color--lightest); padding-top: 1rem;">
  <a href="/esp32/c3/bme280" style="display: flex; align-items: center; gap: 0.5rem; color: var(--md-primary-fg-color); text-decoration: none; font-size: 0.85rem;">
    <span style="font-size: 1.2rem;">←</span>
    <div>
      <div style="color: var(--md-default-fg-color--light); font-size: 0.7rem;">이전</div>
      <div>BME280 sensor</div>
    </div>
  </a>
  <a href="/esp32/c3/wifi_webserver" style="display: flex; align-items: center; gap: 0.5rem; color: var(--md-primary-fg-color); text-decoration: none; font-size: 0.85rem; text-align: right;">
    <div>
      <div style="color: var(--md-default-fg-color--light); font-size: 0.7rem;">다음</div>
      <div>WiFi webserver 🖥️</div>
    </div>
    <span style="font-size: 1.2rem;">→</span>
  </a>
</div>
