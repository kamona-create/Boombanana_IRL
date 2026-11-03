/*
 * BOMBANANA IRL - v1 firmware (simulation build)
 * Module: CABLE + timer + strikes + buzzer
 *
 * Roles: blind (touch), deaf (sight), mute (manual).
 * This firmware only handles electronics. Human roles are the players.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= PINS =================
#define PIN_JACK1   13
#define PIN_JACK2   14
#define PIN_JACK3   16
#define PIN_JACK4   17
#define PIN_BUZZER  18
#define PIN_LED_R   25
#define PIN_LED_G   26
#define PIN_LED_B   27
#define PIN_START    4

// I2C pins for the OLED (ESP32 default I2C bus)
#define I2C_SDA     21
#define I2C_SCL     22
#define OLED_ADDR   0x3C

// ================= GAME CONFIG =================
const uint32_t GAME_SECONDS = 150;   // 2:30
const int TOTAL_ROUNDS      = 5;     // defuse 5 cables to win
const int MAX_STRIKES       = 3;

// A jack pair is wired to GND, so:
//   LOW  = wire still plugged in (intact)
//   HIGH = wire pulled out (cut)
const int JACK_CLOSED = LOW;

const int JACK_PINS[4] = { PIN_JACK1, PIN_JACK2, PIN_JACK3, PIN_JACK4 };
const int LED_PINS[3]  = { PIN_LED_R, PIN_LED_G, PIN_LED_B };

const char* COLOR_NAME[3] = { "RED", "GREEN", "BLUE" };

// Manual table: which wire index (0-based) to cut.
// Update these numbers if you change the printed manual.
const int8_t TABLE_3[3] = { 2, 0, 1 };  // 3 wires: RED->3rd, GREEN->1st, BLUE->2nd
const int8_t TABLE_4[3] = { 3, 2, 1 };  // 4 wires: RED->4th, GREEN->3rd, BLUE->2nd

// ================= DISPLAY =================
Adafruit_SSD1306 display(128, 64, &Wire, -1);

enum State { IDLE, RUNNING, BOOM, WIN };
State state = IDLE;

uint32_t startMs = 0;
int  strikes   = 0;
int  roundNo   = 0;      // 0..TOTAL_ROUNDS-1
int  wireCount = 3;
int  colorIdx  = 0;      // 0 RED, 1 GREEN, 2 BLUE
int  targetWire = 0;
bool armed = false;      // true when all wires are plugged in again
int  lastJack[4] = { LOW, LOW, LOW, LOW };
uint32_t lastDraw = 0;

// ================= BUZZER =================
void buzzerInit() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(PIN_BUZZER, 2000, 8);
#else
  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_BUZZER, 0);
#endif
}

void toneOn(int freq) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWriteTone(PIN_BUZZER, freq);
#else
  ledcWriteTone(0, freq);
#endif
}

void toneOff() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWriteTone(PIN_BUZZER, 0);
#else
  ledcWriteTone(0, 0);
#endif
}

void beep(int freq, int ms) {
  toneOn(freq);
  delay(ms);
  toneOff();
}

void soundStart()  { beep(660, 120); beep(880, 120); beep(1320, 160); }
void soundGood()   { beep(1046, 90);  beep(1318, 120); }
void soundStrike() { beep(150, 500); }
void soundWin()    { beep(523, 140); beep(659, 140); beep(784, 140); beep(1046, 400); }

void soundBoom() {
  for (int f = 800; f >= 80; f -= 20) {
    toneOn(f);
    delay(18);
  }
  toneOff();
  delay(200);
}

// ================= INDICATOR LED =================
void setIndicator(int idx) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[i], (i == idx) ? HIGH : LOW);
  }
}

void indicatorOff() {
  for (int i = 0; i < 3; i++) digitalWrite(LED_PINS[i], LOW);
}

// ================= DRAW =================
void drawIdle() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("BOMBANANA IRL");
  display.setCursor(0, 18);
  display.setTextSize(2);
  display.print("READY");
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("3 players / 150s");
  display.setCursor(0, 52);
  display.print("PRESS START");
  display.display();
}

void drawRunning() {
  uint32_t elapsed = (millis() - startMs) / 1000;
  long remaining = (long)GAME_SECONDS - (long)elapsed;
  if (remaining < 0) remaining = 0;

  char buf[8];
  snprintf(buf, sizeof(buf), "%02ld:%02ld", remaining / 60, remaining % 60);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("CABLE ");
  display.print(roundNo + 1);
  display.print("/");
  display.print(TOTAL_ROUNDS);

  // strikes as X X .
  for (int i = 0; i < MAX_STRIKES; i++) {
    display.setCursor(104 + i * 8, 0);
    display.print(i < strikes ? "X" : ".");
  }

  display.setTextSize(2);
  display.setCursor(0, 12);
  display.print(buf);

  display.setTextSize(1);
  display.setCursor(0, 34);
  display.print("WIRES: ");
  display.print(wireCount);

  display.setCursor(0, 44);
  display.print("LAMP : ");
  display.print(COLOR_NAME[colorIdx]);

  display.setCursor(0, 56);
  if (armed) {
    display.print("PULL ONE WIRE");
  } else {
    display.print("RE-PLUG WIRES");
  }

  display.display();
}

void drawEnd(const char* big, const char* small) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 8);
  display.print(big);
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print(small);
  display.setCursor(0, 54);
  display.print("PRESS START");
  display.display();
}

// ================= GAME LOGIC =================
int readJack(int i) {
  return digitalRead(JACK_PINS[i]);
}

bool allPlugged() {
  for (int i = 0; i < 4; i++) {
    if (readJack(i) != JACK_CLOSED) return false;
  }
  return true;
}

void newRound() {
  wireCount  = (random(2) == 0) ? 3 : 4;
  colorIdx   = random(3);
  targetWire = (wireCount == 3) ? TABLE_3[colorIdx] : TABLE_4[colorIdx];
  armed      = false;
  setIndicator(colorIdx);
}

void startGame() {
  strikes = 0;
  roundNo = 0;
  startMs = millis();
  state = RUNNING;
  newRound();
  soundStart();
}

void gameOver() {
  state = BOOM;
  indicatorOff();
  soundBoom();
  drawEnd("BOOM!", "Bomb exploded");
}

void gameWin() {
  state = WIN;
  indicatorOff();
  soundWin();
  drawEnd("DEFUSED", "You saved the day");
}

void handleCut(int idx) {
  if (idx == targetWire) {
    roundNo++;
    if (roundNo >= TOTAL_ROUNDS) {
      gameWin();
      return;
    }
    soundGood();
    newRound();
  } else {
    strikes++;
    soundStrike();
    if (strikes >= MAX_STRIKES) {
      gameOver();
      return;
    }
    newRound();
  }
}

void handleStartButton() {
  static bool lastPressed = false;
  bool pressed = (digitalRead(PIN_START) == LOW);
  if (pressed && !lastPressed) {
    if (state == IDLE) {
      startGame();
    } else if (state == BOOM || state == WIN) {
      delay(300);
      startGame();
    }
  }
  lastPressed = pressed;
}

void updateTimer() {
  uint32_t elapsed = (millis() - startMs) / 1000;
  long remaining = (long)GAME_SECONDS - (long)elapsed;
  if (remaining <= 0) {
    gameOver();
    return;
  }

  static long lastBeepSecond = -1;
  if (remaining != lastBeepSecond) {
    lastBeepSecond = remaining;
    if (remaining > 60)      beep(2000, 8);
    else if (remaining > 20) beep(2200, 10);
    else                     beep(2600, 12);
  }
}

// ================= SETUP / LOOP =================
void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("  found device at 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Scan done.");
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_START, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(JACK_PINS[i], INPUT_PULLUP);
  for (int i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  buzzerInit();

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 not found. Check: SDA=21, SCL=22, VCC=3V3, GND=GND");
    scanI2C();
  } else {
    Serial.println("SSD1306 OK");
  }
  display.clearDisplay();
  display.display();

  randomSeed(micros());

  for (int i = 0; i < 4; i++) lastJack[i] = readJack(i);

  state = IDLE;
  drawIdle();
}

void loop() {
  handleStartButton();

  if (state == RUNNING) {
    updateTimer();
    if (state != RUNNING) return;

    if (!armed && allPlugged()) {
      armed = true;
    }

    for (int i = 0; i < 4; i++) {
      int v = readJack(i);
      bool nowCut  = (v != JACK_CLOSED);
      bool wasCut  = (lastJack[i] != JACK_CLOSED);
      lastJack[i]  = v;

      if (armed && nowCut && !wasCut && i < wireCount) {
        handleCut(i);
        break;
      }
    }

    if (state == RUNNING && millis() - lastDraw > 200) {
      lastDraw = millis();
      drawRunning();
    }
  }
}
