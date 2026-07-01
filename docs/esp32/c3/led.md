# Hello, LED 🜨

> 모든 프로그래밍 입문이 "Hello, World!"로 시작하듯, 피지컬 컴퓨팅 입문은 LED 블링킹으로 시작합니다.

ESP32-C3 SuperMini에는 GPIO8에 내장 LED(상태표시용, 파랑)가 달려 있습니다. 코드 몇 줄로 이 LED를 껐다 켰다 할 수 있다는 게 별것 아닌 것 같지만, 처음으로 "내가 짠 코드가 물리적인 무언가를 움직였다"는 감각은 생각보다 꽤 강렬합니다.

---

## 예제 1 — 기본 블링킹

```cpp
int led = 8; // GPIO8 — SuperMini 내장 LED

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH); // LED 켜짐
  delay(1000);             // 1초 대기
  digitalWrite(led, LOW);  // LED 꺼짐
  delay(1000);             // 1초 대기
}
```

아두이노 IDE에서 C3로 업로드하면 1초 간격으로 파란 LED가 깜빡입니다.

### LED_BUILTIN을 쓰지 않는 이유

아두이노 공식 Blink 예제는 핀 번호 대신 `LED_BUILTIN`이라는 매크로를 사용합니다.

```cpp
pinMode(LED_BUILTIN, OUTPUT); // 공식 예제 방식
```

우노 같은 공식 보드에서는 이 매크로가 정확한 핀 번호로 자동 연결되지만, SuperMini는 비공식 서드파티 보드라 보드 `LED_BUILTIN`이 엉뚱한 핀을 가리킬 수 있습니다.

실제로 처음 공식 예제를 그대로 올렸을 때 LED가 깜빡이지 않고 그냥 켜져만 있었습니다. 코드는 엉뚱한 핀을 열심히 껐다 켰다 하고 있었고, GPIO8은 건드리지도 않은 채 부팅 시 기본값(HIGH)으로 그냥 켜져있었던 것이었습니다.

> **SuperMini에서는 `LED_BUILTIN` 대신 핀 번호 `8`을 직접 명시해야합니다.**

### delay 값을 바꿔보세요

	`delay(1000)`의 숫자는 밀리초(ms) 단위입니다. 1000 ms = 1 초.

```cpp
delay(100);  // 0.1초 — 빠르게 깜빡임
delay(500);  // 0.5초
delay(2000); // 2초 — 천천히 깜빡임
```

숫자 하나 바꾸고 다시 업로드하면 바로 반응합니다. 코드가 하드웨어를 직접 제어하고 있다는 감각을 익히기에 가장 간단한 실험입니다.

---

## 예제 2 — 페이드인/페이드아웃 (Breathing)

단순히 켜고 끄는 것에서 한 단계 나아가, LED를 서서히 켜고 끄는 PWM 제어입니다. 크리스마스 전구처럼 숨쉬듯 깜빡이는 패턴을 만들 수 있습니다.

```cpp
int led = 8;

void setup() {
  ledcAttach(led, 5000, 8); // 핀, 주파수(Hz), 해상도(bit)
}

void loop() {
  for (int i = 255; i >= 0; i--) {
    ledcWrite(led, i);
    delay(10);
  }
  for (int i = 0; i <= 255; i++) {
    ledcWrite(led, i);
    delay(10);
  }
  delay(1000); // 꺼진 후 1초 대기
}
```

### analogWrite 대신 ledcWrite를 쓰는 이유

아두이노 UNO에서는 `analogWrite()`로 PWM을 제어하지만, ESP32 아두이노 코어 3.x 버전부터는 `ledcAttach()` + `ledcWrite()` 방식을 사용합니다. `analogWrite()`를 쓰면 컴파일 에러가 나므로 주의합니다.

> `ledcAttach(핀, 주파수, 해상도)` 한 줄로 설정하고, `ledcWrite(핀, 0~255)`로 밝기를 제어합니다.

---

## 예제 3 — 부트 버튼으로 LED 제어

SuperMini의 BOOT 버튼(GPIO9)은 업로드 모드 진입 용도이지만, 펌웨어가 정상 실행 중일 때는 일반 입력 핀으로 자유롭게 사용할 수 있습니다.

```cpp
int led = 8;
int bootBtn = 9;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(bootBtn, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(bootBtn) == LOW) {
    digitalWrite(led, LOW);  // 누르면 켜짐
  } else {
    digitalWrite(led, HIGH); // 떼면 꺼짐
  }
}
```

### active-LOW 주의

버튼이 눌리면 `LOW`, 떼면 `HIGH`로 읽힙니다(`INPUT_PULLUP` 방식이라 반전). LED도 마찬가지로 반전 연결이라, 처음엔 누르면 꺼지고 떼면 켜지는 반대 동작이 나올 수 있습니다. HIGH/LOW를 반대로 적어주면 해결됩니다.

---

## 정리하며

LED 하나를 깜빡이는 게 전부였지만, 여기서 익힌 구조 — `setup()`에서 핀 설정, `loop()`에서 반복 제어, 버튼 입력 읽기 — 는 앞으로 센서를 읽고 측정값이 임계치를 넘으면 신호를 보내는 코드까지 그대로 이어집니다.

다음은 첫 번째 센서, DHT11 (기초 온습도 센서)로 넘어갑니다.