/*
 * BOMBANANA IRL - v1 firmware
 * Modules: CABLE (4 banana jacks) + SWITCH panel + timer + strikes + buzzer
 *
 * Roles are human: blind (touch), deaf (sight), mute (reads the manual).
 * This firmware only handles the electronics.
 *
 * Display : 2.4" ILI9341 SPI, 320x240 landscape (no touch)
 * LEDs    : 5x WS2812B addressable on ONE data pin
 *           LED 0      = big "lamp" colour for the round
 *           LED 1..4   = socket indicators (green = plugged, off = cut)
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>

// ================= PINS =================
// Banana jacks: side A -> GPIO, side B -> GND
#define PIN_JACK1   13
#define PIN_JACK2   14
#define PIN_JACK3   16
#define PIN_JACK4   17

#define PIN_BUZZER  32      // was 18 - 18 is VSPI SCK for the TFT
#define PIN_START   33
#define PIN_LEDS    27      // WS2812B data (all 5 LEDs daisy-chained)

// TFT (set in platformio.ini build_flags):
//   MOSI 23 | MISO 19 | SCK 18 | CS 5 | DC 2 | RST 4 | BL -> 3V3

#define NUM_LEDS    5
#define LED_LAMP    0
#define LED_FIRST   1       // LEDs 1..4 = jacks 1..4

// ================= GAME CONFIG =================
const uint32_t GAME_SECONDS = 150;   // 2:30
const int TOTAL_ROUNDS      = 5;     // defuse 5 cables to win
const int MAX_STRIKES       = 3;

// A jack pair is wired to GND, so:
//   LOW  = wire still plugged in (intact)
//   HIGH = wire pulled out (cut)
const int JACK_CLOSED = LOW;

const int JACK_PINS[4] = { PIN_JACK1, PIN_JACK2, PIN_JACK3, PIN_JACK4 };

const char* COLOR_NAME[3] = { "RED", "GREEN", "BLUE" };
const uint8_t COLOR_RGB[3][3] = {
  { 255,   0,   0 },
  {   0, 200,   0 },
  {   0,   0, 255 },
};

// Manual table: which wire index (0-based) to cut.
// These MUST match the printed manual.
const int8_t TABLE_3[3] = { 2, 0, 1 };  // 3 wires: RED->3rd, GREEN->1st, BLUE->2nd
const int8_t TABLE_4[3] = { 3, 2, 1 };  // 4 wires: RED->4th, GREEN->3rd, BLUE->2nd

// ================= OBJECTS =================
TFT_eSPI tft = TFT_eSPI();
Adafruit_NeoPixel leds(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

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

// what is currently on screen (so we only redraw what changed)
long lastShownSec     = -1;
int  lastShownStrikes = -1;
int  lastShownRound   = -1;
int  lastShownWires   = -1;
bool lastShownArmed   = false;
int  lastShownColor   = -1;

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

// ================= LEDS =================
void ledsBegin() {
  leds.begin();
  leds.setBrightness(60);      // keep current low, 5 LEDs on USB power
  leds.clear();
  leds.show();
}

void setLamp(int idx) {
  leds.setPixelColor(LED_LAMP, leds.Color(COLOR_RGB[idx][0],
                                          COLOR_RGB[idx][1],
                                          COLOR_RGB[idx][2]));
  leds.show();
}

void updateSocketLeds() {
  for (int i = 0; i < 4; i++) {
    bool plugged = (digitalRead(JACK_PINS[i]) == JACK_CLOSED);
    bool used    = (i < wireCount);
    if (plugged && used)      leds.setPixelColor(LED_FIRST + i, leds.Color(0, 40, 0));
    else if (plugged)         leds.setPixelColor(LED_FIRST + i, leds.Color(10, 10, 10));
    else                      leds.setPixelColor(LED_FIRST + i, 0);
  }
  leds.show();
}

void ledsOff() {
  leds.clear();
  leds.show();
}

void ledsFlash(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) leds.setPixelColor(i, leds.Color(r, g, b));
  leds.show();
}

// ================= DRAW =================
void drawIdle() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("BOMBANANA IRL", 160, 50, 4);
  tft.drawString("READY", 160, 110, 7);
  tft.drawString("3 players  /  150 seconds", 160, 175, 2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("PRESS START", 160, 210, 2);

  lastShownSec = -1;
  lastShownStrikes = -1;
  lastShownRound = -1;
  lastShownWires = -1;
  lastShownArmed = false;
  lastShownColor = -1;
}

void drawTimer(long remaining) {
  char buf[8];
  snprintf(buf, sizeof(buf), "%02ld:%02ld", remaining / 60, remaining % 60);
  uint16_t col = (remaining <= 20) ? TFT_RED : TFT_WHITE;
  tft.fillRect(20, 80, 280, 70, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(col, TFT_BLACK);
  tft.drawString(buf, 160, 115, 7);
  lastShownSec = remaining;
}

void drawStatus() {
  // header
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("BOMBANANA IRL", 8, 8, 2);

  // strikes, top right
  char st[8];
  for (int i = 0; i < MAX_STRIKES; i++) st[i] = (i < strikes) ? 'X' : '-';
  st[MAX_STRIKES] = 0;
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(strikes ? TFT_RED : TFT_DARKGREY, TFT_BLACK);
  tft.drawString(st, 312, 8, 2);

  // bottom rows
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char row1[24];
  snprintf(row1, sizeof(row1), "CABLE %d/%d", roundNo + 1, TOTAL_ROUNDS);
  tft.fillRect(0, 155, 200, 26, TFT_BLACK);
  tft.drawString(row1, 8, 158, 2);

  char row1b[16];
  snprintf(row1b, sizeof(row1b), "WIRES: %d", wireCount);
  tft.fillRect(200, 155, 120, 26, TFT_BLACK);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(row1b, 312, 158, 2);

  tft.setTextDatum(TL_DATUM);
  char row2[24];
  snprintf(row2, sizeof(row2), "LAMP: %s", COLOR_NAME[colorIdx]);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.fillRect(0, 185, 170, 26, TFT_BLACK);
  tft.drawString(row2, 8, 188, 2);

  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(armed ? TFT_GREEN : TFT_ORANGE, TFT_BLACK);
  tft.fillRect(170, 185, 150, 26, TFT_BLACK);
  tft.drawString(armed ? "PULL ONE WIRE" : "PLUG ALL WIRES", 312, 188, 2);

  tft.setTextDatum(TL_DATUM);
  lastShownStrikes = strikes;
  lastShownRound   = roundNo;
  lastShownWires   = wireCount;
  lastShownArmed   = armed;
  lastShownColor   = colorIdx;
}

void drawEnd(const char* big, const char* small, uint16_t col) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(col, TFT_BLACK);
  tft.drawString(big, 160, 100, 7);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(small, 160, 165, 2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("PRESS START", 160, 200, 2);

  lastShownSec = -1;
  lastShownStrikes = -1;
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
  updateSocketLeds();
  setLamp(colorIdx);
}

void startGame() {
  strikes = 0;
  roundNo = 0;
  startMs = millis();
  state = RUNNING;
  tft.fillScreen(TFT_BLACK);
  newRound();
  drawStatus();
  soundStart();
}

void gameOver() {
  state = BOOM;
  soundBoom();
  ledsFlash(255, 0, 0);
  drawEnd("BOOM!", "The bomb exploded", TFT_RED);
}

void gameWin() {
  state = WIN;
  soundWin();
  ledsFlash(0, 200, 0);
  drawEnd("DEFUSED", "You saved the day", TFT_GREEN);
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
      ledsOff();
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
    drawTimer(remaining);
  }
}

// ================= SETUP / LOOP =================
void setup() {
  Serial.begin(115200);

  pinMode(PIN_START, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(JACK_PINS[i], INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);           // 320x240 landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextWrap(false);

  ledsBegin();
  buzzerInit();

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

    bool jackChanged = false;
    for (int i = 0; i < 4; i++) {
      int v = readJack(i);
      if (v != lastJack[i]) jackChanged = true;
      lastJack[i] = v;
    }

    if (!armed && allPlugged()) {
      armed = true;
      drawStatus();
    }

    if (jackChanged) {
      updateSocketLeds();
      for (int i = 0; i < 4; i++) {
        int v = readJack(i);
        bool nowCut = (v != JACK_CLOSED);
        if (armed && nowCut && i < wireCount) {
          handleCut(i);
          break;
        }
      }
    }

    if (state == RUNNING) {
      if (strikes  != lastShownStrikes ||
          roundNo  != lastShownRound   ||
          wireCount != lastShownWires  ||
          colorIdx != lastShownColor) {
        drawStatus();
      }
    }
  }
}
