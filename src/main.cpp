/*
 * BOMBANANA IRL - v1 firmware
 * Modules: CABLE (4 banana jacks) + SWITCH (4 toggles) + timer + strikes + buzzer
 *
 * Roles are human: blind (touch), deaf (sight), mute (reads the manual).
 * This firmware only handles the electronics.
 *
 * Display : 2.4" ILI9341 SPI, 320x240 landscape (no touch)
 * LEDs    : 1x RGB LED (the big "lamp") + 4x green LEDs (socket indicators,
 *           reused as per-switch feedback during a switch round)
 *           only the lamp needs colour, so no addressable LEDs and no extra library
 *
 * Round plan: 5 rounds, 2 of them switch rounds, shuffled each game.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

// ================= PINS =================
// Banana jacks: side A -> GPIO, side B -> GND
#define PIN_JACK1   13
#define PIN_JACK2   14
#define PIN_JACK3   16
#define PIN_JACK4   17

#define PIN_BUZZER  32      // was 18 - 18 is VSPI SCK for the TFT
#define PIN_START   33

// Big lamp = one RGB LED. Sockets = one green LED each (plugged / cut).
// 4 / 21 / 22 / 12 are the only normal GPIOs left after the TFT and the jacks.
// GPIO12 is a strapping pin but the LED+resistor holds it LOW at boot, which is
// exactly what the bootloader wants (3.3 V flash). GPIO15 stays unused.
#define PIN_LAMP_R  25
#define PIN_LAMP_G  26
#define PIN_LAMP_B  27
#define PIN_SOCK1    4
#define PIN_SOCK2   21
#define PIN_SOCK3   22
#define PIN_SOCK4   12

// Switch module: 4x MTS-203 toggle, one pole each, wired as a plain SPST to GND.
// 34 / 35 / 36 / 39 are the only free pins left, but they are INPUT-ONLY and
// have NO internal pull-up. Each one therefore needs an external 10k to 3V3
// on the perfboard. Without that resistor the pin floats and reads garbage.
//   switch open  -> pulled to 3V3 -> HIGH -> "UP"
//   switch closed-> shorted to GND -> LOW  -> "DOWN"
#define PIN_SW1     34
#define PIN_SW2     35
#define PIN_SW3     36
#define PIN_SW4     39

// TFT (set in platformio.ini build_flags):
//   MOSI 23 | MISO 19 | SCK 18 | CS 5 | DC 2 | RST -> 3V3 | BL -> 3V3

// ================= GAME CONFIG =================
const uint32_t GAME_SECONDS = 150;   // 2:30
const int TOTAL_ROUNDS      = 5;     // 5 modules to clear to win
const int SWITCH_ROUNDS     = 2;     // of the 5, this many are switch rounds
const int MAX_STRIKES       = 3;

// A switch round is cleared once all four switches sit on the target pattern
// and stay there for this long. The hold stops a switch that is mid-throw
// (contacts still bouncing) from counting as a match.
const uint32_t SWITCH_HOLD_MS = 500;

// A jack pair is wired to GND, so:
//   LOW  = wire still plugged in (intact)
//   HIGH = wire pulled out (cut)
const int JACK_CLOSED = LOW;

const int JACK_PINS[4] = { PIN_JACK1, PIN_JACK2, PIN_JACK3, PIN_JACK4 };

// ================= JACK SENSING =================
// A banana wire is a plain short: red plug = signal, black plug = GND.
//   wire in  -> GPIO pulled to GND -> LOW
//   wire out -> nothing holds it   -> internal pull-up -> HIGH
// The pin is never driven by the ESP32, it only reads. A dead short to GND
// through the internal ~45k pull-up is about 0.07 mA, so it cannot damage it.
//
// Contacts bounce for a few ms when a plug is pushed in or pulled out, and a
// blindfolded player may grab two wires at once. So every pin is debounced
// (30 ms) here and nowhere else in the code.
const uint32_t DEBOUNCE_MS = 30;

struct JackState {
  bool     plugged;    // debounced: true = wire is in
  bool     raw;        // last raw sample
  uint32_t changedMs;  // when raw last moved
  bool     cutEvent;   // latched: a wire was pulled, not handled yet
};

JackState jacks[4];

void jacksBegin() {
  for (int i = 0; i < 4; i++) {
    pinMode(JACK_PINS[i], INPUT_PULLUP);
    jacks[i].plugged   = true;          // assume all wires are in at power-on
    jacks[i].raw       = true;
    jacks[i].changedMs = millis();
    jacks[i].cutEvent  = false;
  }
}

void jacksPoll() {
  uint32_t now = millis();
  for (int i = 0; i < 4; i++) {
    bool raw = (digitalRead(JACK_PINS[i]) == JACK_CLOSED);
    if (raw != jacks[i].raw) {
      jacks[i].raw       = raw;
      jacks[i].changedMs = now;
    }
    // only accept the new value once it has been stable for DEBOUNCE_MS
    if (jacks[i].plugged != jacks[i].raw &&
        (now - jacks[i].changedMs) >= DEBOUNCE_MS) {
      jacks[i].plugged = jacks[i].raw;
      if (!jacks[i].plugged) jacks[i].cutEvent = true;   // in -> out = a cut
    }
  }
}

bool allPlugged() {
  for (int i = 0; i < 4; i++) {
    if (!jacks[i].plugged) return false;
  }
  return true;
}

// Returns the first unhandled cut, or -1. The event is consumed either way,
// so a cut made while the board is not armed can never score later.
int takeCut() {
  for (int i = 0; i < 4; i++) {
    if (jacks[i].cutEvent) {
      jacks[i].cutEvent = false;
      return i;
    }
  }
  return -1;
}

// ================= SWITCH SENSING =================
// Same debounce as the jacks, but there is no edge event to latch: a toggle
// only matters while the round is being checked, so we just keep the stable
// level. Bit i of swState is 1 when switch i is UP.
const int SWITCH_PINS[4] = { PIN_SW1, PIN_SW2, PIN_SW3, PIN_SW4 };

uint8_t  swState      = 0;     // debounced, bit 1 = UP
uint8_t  swLastRaw    = 0;     // last raw sample
uint32_t swChangedMs  = 0;     // when the raw sample last moved

void switchesBegin() {
  for (int i = 0; i < 4; i++) {
    pinMode(SWITCH_PINS[i], INPUT);   // no INPUT_PULLUP - pin has no pull-up
    if (digitalRead(SWITCH_PINS[i]) == HIGH) swState |= (1 << i);
  }
  swLastRaw   = swState;
  swChangedMs = millis();
}

void switchesPoll() {
  uint32_t now = millis();
  uint8_t  raw = 0;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(SWITCH_PINS[i]) == HIGH) raw |= (1 << i);
  }
  if (raw != swLastRaw) {
    swLastRaw   = raw;
    swChangedMs = now;
  }
  if (raw != swState && (now - swChangedMs) >= DEBOUNCE_MS) {
    swState = raw;
  }
}

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
const int SOCK_PINS[4] = { PIN_SOCK1, PIN_SOCK2, PIN_SOCK3, PIN_SOCK4 };

enum State { IDLE, RUNNING, BOOM, WIN };
State state = IDLE;

enum RoundType { RND_CABLE, RND_SWITCH };
RoundType roundPlan[TOTAL_ROUNDS];
bool      roundIsSwitch = false;

uint32_t startMs = 0;
int  strikes   = 0;
int  roundNo   = 0;      // 0..TOTAL_ROUNDS-1
int  wireCount = 3;
int  colorIdx  = 0;      // 0 RED, 1 GREEN, 2 BLUE
int  targetWire = 0;
bool armed = false;      // true when all wires are plugged in again

uint8_t  swTarget   = 0;     // 4-bit pattern the player must reach
bool     swHolding  = false; // we are inside the SWITCH_HOLD_MS window
uint32_t swHoldStart = 0;

// what is currently on screen (so we only redraw what changed)
long lastShownSec     = -1;
int  lastShownStrikes = -1;
int  lastShownRound   = -1;
int  lastShownWires   = -1;
bool lastShownArmed   = false;
int  lastShownColor   = -1;
int  lastShownSw      = -1;

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
void ledsOff();   // defined below, used by ledsBegin()

void ledsBegin() {
  pinMode(PIN_LAMP_R, OUTPUT);
  pinMode(PIN_LAMP_G, OUTPUT);
  pinMode(PIN_LAMP_B, OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(SOCK_PINS[i], OUTPUT);
  ledsOff();
}

void setLamp(int idx) {
  digitalWrite(PIN_LAMP_R, COLOR_RGB[idx][0] ? HIGH : LOW);
  digitalWrite(PIN_LAMP_G, COLOR_RGB[idx][1] ? HIGH : LOW);
  digitalWrite(PIN_LAMP_B, COLOR_RGB[idx][2] ? HIGH : LOW);
}

void updateSocketLeds() {
  for (int i = 0; i < 4; i++) {
    bool used = (i < wireCount);
    digitalWrite(SOCK_PINS[i], (jacks[i].plugged && used) ? HIGH : LOW);
  }
}

// During a switch round the same four green LEDs become per-switch feedback:
// a lit LED means that switch is already on its target position. The blind
// player cannot use it, but the sighted one can call the remaining ones out.
void updateSwitchLeds() {
  for (int i = 0; i < 4; i++) {
    bool ok = (bitRead(swState, i) == bitRead(swTarget, i));
    digitalWrite(SOCK_PINS[i], ok ? HIGH : LOW);
  }
}

void lampOff() {
  digitalWrite(PIN_LAMP_R, LOW);
  digitalWrite(PIN_LAMP_G, LOW);
  digitalWrite(PIN_LAMP_B, LOW);
}

void ledsOff() {
  digitalWrite(PIN_LAMP_R, LOW);
  digitalWrite(PIN_LAMP_G, LOW);
  digitalWrite(PIN_LAMP_B, LOW);
  for (int i = 0; i < 4; i++) digitalWrite(SOCK_PINS[i], LOW);
}

// Flash the whole set one colour (boom = red, win = green).
void ledsFlash(uint8_t r, uint8_t g, uint8_t b) {
  digitalWrite(PIN_LAMP_R, r ? HIGH : LOW);
  digitalWrite(PIN_LAMP_G, g ? HIGH : LOW);
  digitalWrite(PIN_LAMP_B, b ? HIGH : LOW);
  bool on = (r || g || b);
  for (int i = 0; i < 4; i++) digitalWrite(SOCK_PINS[i], on ? HIGH : LOW);
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

// Switch rounds show four UP/DN cells instead of the colour block. The boxes
// are drawn in green when that switch already matches, orange when it does
// not, so the sighted player can read the progress without reading numbers.
void drawSwitchStatus() {
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("BOMBANANA IRL", 8, 8, 2);

  char st[8];
  for (int i = 0; i < MAX_STRIKES; i++) st[i] = (i < strikes) ? 'X' : '-';
  st[MAX_STRIKES] = 0;
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(strikes ? TFT_RED : TFT_DARKGREY, TFT_BLACK);
  tft.drawString(st, 312, 8, 2);

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char row1[24];
  snprintf(row1, sizeof(row1), "SWITCH %d/%d", roundNo + 1, TOTAL_ROUNDS);
  tft.fillRect(0, 152, 190, 26, TFT_BLACK);
  tft.drawString(row1, 8, 155, 2);

  int matched = 0;
  for (int i = 0; i < 4; i++) {
    if (bitRead(swState, i) == bitRead(swTarget, i)) matched++;
  }
  char row2[24];
  snprintf(row2, sizeof(row2), "MATCH %d/4", matched);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(matched == 4 ? TFT_GREEN : TFT_YELLOW, TFT_BLACK);
  tft.fillRect(190, 152, 122, 26, TFT_BLACK);
  tft.drawString(row2, 312, 155, 2);

  for (int i = 0; i < 4; i++) {
    int  x  = 8 + i * 78;
    bool up = bitRead(swState, i);
    uint16_t c = (up == bitRead(swTarget, i)) ? TFT_GREEN : TFT_ORANGE;
    tft.fillRect(x, 183, 70, 46, TFT_BLACK);
    tft.drawRect(x, 183, 70, 46, c);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(c, TFT_BLACK);
    tft.drawString(up ? "UP" : "DN", x + 35, 206, 4);
  }

  lastShownStrikes = strikes;
  lastShownRound   = roundNo;
  lastShownSw      = swState;
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
// The 5 rounds are not all cables: SWITCH_ROUNDS of them are switch modules.
// The plan is built once per game and then shuffled, so the order cannot be
// memorised between plays.
void buildRoundPlan() {
  for (int i = 0; i < TOTAL_ROUNDS; i++) {
    roundPlan[i] = (i < SWITCH_ROUNDS) ? RND_SWITCH : RND_CABLE;
  }
  for (int i = TOTAL_ROUNDS - 1; i > 0; i--) {
    int j = random(i + 1);
    RoundType t  = roundPlan[i];
    roundPlan[i] = roundPlan[j];
    roundPlan[j] = t;
  }
}

void newCableRound() {
  wireCount  = (random(2) == 0) ? 3 : 4;
  colorIdx   = random(3);
  targetWire = (wireCount == 3) ? TABLE_3[colorIdx] : TABLE_4[colorIdx];
  armed      = false;
  updateSocketLeds();
  setLamp(colorIdx);
}

// random(1, 16) never gives 0 (all down), and 15 (all up) is rejected too:
// both are solved by throwing every switch the same way without reading
// anything, which is not a puzzle.
void newSwitchRound() {
  do { swTarget = (uint8_t)random(1, 16); } while (swTarget == 15);
  armed     = false;
  swHolding = false;
  lampOff();
  updateSwitchLeds();
}

void nextRound() {
  roundIsSwitch = (roundPlan[roundNo] == RND_SWITCH);
  // Both layouts share rows 152..240 but draw different things there, so wipe
  // the whole band. The timer lives above it and keeps counting untouched.
  tft.fillRect(0, 152, 320, 88, TFT_BLACK);
  if (roundIsSwitch) {
    newSwitchRound();
    drawSwitchStatus();
  } else {
    newCableRound();
    drawStatus();
  }
}

void startGame() {
  strikes = 0;
  roundNo = 0;
  startMs = millis();
  state = RUNNING;
  tft.fillScreen(TFT_BLACK);
  buildRoundPlan();
  nextRound();
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
    nextRound();
  } else {
    strikes++;
    soundStrike();
    if (strikes >= MAX_STRIKES) {
      gameOver();
      return;
    }
    nextRound();
  }
}

// Called every loop while a switch round is running. A switch thrown on a dark
// game is silent and weightless, so the only feedback is the screen and the
// four green LEDs. Nothing here can strike the player: time is the pressure.
void handleSwitchRound() {
  uint32_t now = millis();

  if (swState == swTarget) {
    if (!swHolding) {
      swHolding   = true;
      swHoldStart = now;
    } else if (now - swHoldStart >= SWITCH_HOLD_MS) {
      swHolding = false;
      roundNo++;
      if (roundNo >= TOTAL_ROUNDS) {
        gameWin();
        return;
      }
      soundGood();
      nextRound();
      return;
    }
  } else {
    swHolding = false;
  }

  updateSwitchLeds();

  if (swState != lastShownSw ||
      strikes != lastShownStrikes ||
      roundNo != lastShownRound) {
    drawSwitchStatus();
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
  jacksBegin();
  switchesBegin();

  tft.init();
  tft.setRotation(1);           // 320x240 landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextWrap(false);

  ledsBegin();
  buzzerInit();

  randomSeed(micros());

  state = IDLE;
  drawIdle();
}

void loop() {
  handleStartButton();
  if (state != RUNNING) return;

  updateTimer();
  if (state != RUNNING) return;

  jacksPoll();
  switchesPoll();

  // ---- switch module ----
  if (roundIsSwitch) {
    takeCut();            // drain, so a pulled wire cannot score later
    handleSwitchRound();
    return;
  }

  // ---- cable module ----
  updateSocketLeds();

  if (!armed && allPlugged()) {
    armed = true;
    drawStatus();
  }

  int cut = takeCut();
  if (cut >= 0 && armed && cut < wireCount) {
    handleCut(cut);
    return;               // nextRound() has already drawn the next round
  }

  if (strikes   != lastShownStrikes ||
      roundNo   != lastShownRound   ||
      wireCount != lastShownWires   ||
      colorIdx  != lastShownColor) {
    drawStatus();
  }
}
