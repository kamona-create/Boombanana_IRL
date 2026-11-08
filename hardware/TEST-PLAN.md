# Hardware test plan — hand-off to the build project

> This repo contains **no assembled hardware**. The firmware has only been
> proven by compiling (RAM 6.8 %, flash 23.6 %) and by the browser simulator
> in [`../sim/index.html`](../sim/index.html). Every check below is written for
> the separate build project, once the parts exist and the board is soldered.

Order matters: do not skip to the game until §1 and §2 pass. A wiring fault
looks exactly like a firmware bug on screen.

---

## 0. Before power

| # | Check | Pass |
|---|---|---|
| 0.1 | Continuity: every GND point to the ESP32 GND pin | ≤ 0.5 Ω |
| 0.2 | No short between 5V and GND, or 3V3 and GND | > 1 kΩ |
| 0.3 | 10 kΩ pull-ups present on GPIO 34 / 35 / 36 / 39 | 9–11 kΩ to 3V3 |
| 0.4 | 1 kΩ series on each of GPIO 13 / 14 / 16 / 17 | 0.9–1.1 kΩ |
| 0.5 | 100 nF from each jack signal node to GND | reads open on a DMM (normal) |
| 0.6 | LED polarity: cathode to GND, anode through 220 Ω | diode test lights |
| 0.7 | TFT RST tied to 3V3, BL tied to 3V3 | continuity |
| 0.8 | Nothing conductive touching the exposed jack bodies | visual |

## 1. Power-up (no game)

| # | Action | Expected |
|---|---|---|
| 1.1 | Connect 5 V from the USB-C panel extension | backlight on, boot screen `BOMBANANA IRL / READY` |
| 1.2 | Watch the serial monitor at 115200 | boot log, no brownout reset loop |
| 1.3 | Touch nothing for 10 s | screen stays on the idle screen, no random strikes |
| 1.4 | Measure 3V3 rail | 3.25–3.35 V |
| 1.5 | Disconnect and reconnect power 3× | boots every time, no stuck white screen |

If the screen is white or black: check CS 5, DC 2, MOSI 23, SCK 18, and that
`platformio.ini` still has `TFT_RST=-1`.

## 2. Inputs — one at a time

### 2.1 Jacks (GPIO 13 / 14 / 16 / 17)

| # | Action | Expected |
|---|---|---|
| 2.1.1 | All 8 wires plugged, press START | `PLUG ALL WIRES` is **not** shown; the round shows `PULL ONE WIRE` |
| 2.1.2 | Pull jack 1 wire | one wire disappears from the "plugged" set, no other jack changes |
| 2.1.3 | Plug it back | set returns to full |
| 2.1.4 | Grab two wires and pull together | counts as **one** cut, not two |
| 2.1.5 | Repeat 2.1.2–2.1.4 for jacks 2, 3, 4 | independent, no cross-talk |
| 2.1.6 | Wiggle a plugged wire without removing it | no cut (30 ms debounce) |

### 2.2 Switches (GPIO 34 / 35 / 36 / 39)

| # | Action | Expected |
|---|---|---|
| 2.2.1 | Flip switch 1 up and down | only switch 1's indicator LED changes |
| 2.2.2 | Repeat for switches 2–4 | one LED per switch |
| 2.2.3 | Leave all four in one position for 1 s | `MATCH` value is stable, no flicker |
| 2.2.4 | Flip a switch fast 10× | counts once per real change, no bounce |

### 2.3 Start button (GPIO 33)

| # | Action | Expected |
|---|---|---|
| 2.3.1 | Press START on the idle screen | timer starts at 2:30 |
| 2.3.2 | Press START during a round | ignored (does not restart) |
| 2.3.3 | Press START after BOOM / DEFUSED | returns to a fresh game |

## 3. Outputs

| # | Action | Expected |
|---|---|---|
| 3.1 | Each round start | short success/start tone from the buzzer |
| 3.2 | Wrong wire | low error tone + one strike `X` |
| 3.3 | Three strikes | red flash + falling explosion tone, screen shows `BOOM!` |
| 3.4 | Clear 5 rounds | green flash + win tone, screen shows `DEFUSED` |
| 3.5 | Cable round | RGB lamp shows exactly the colour named on screen |
| 3.6 | Switch round | green LED per correct switch, off per wrong switch |
| 3.7 | Timer hits 0:00 | BOOM, no further input accepted |

## 4. Full-game soak

| # | Action | Expected |
|---|---|---|
| 4.1 | Play 3 full games back to back | no reset, no stuck state, same behaviour |
| 4.2 | Timer accuracy | 150 s measured with a stopwatch, ±1 s |
| 4.3 | Round plan | always 5 rounds, exactly 2 switch rounds, order differs between games |
| 4.4 | Switch target | never `0000` and never `1111` |
| 4.5 | Leave it running for 30 min idle | screen stays on, no watchdog reset |

## 5. Before it goes in the case

| # | Check | Pass |
|---|---|---|
| 5.1 | Every JST-XH connector latches and is keyed | cannot be reversed |
| 5.2 | No solder joint can touch the jack bodies or the panel | visual + continuity |
| 5.3 | Board and TFT clear the jack bodies by ≥ 2 mm | ruler |
| 5.4 | Nothing strains a wire when the lid goes on | flex test |
| 5.5 | 10-minute play session with power bank only | no overheating, no brownout |

## 6. Known firmware behaviour to confirm, not "fix"

- A **wrong** wire still clears the cable round — it costs one strike, not a retry.
- After every cable round (right or wrong) the next round waits for
  `PLUG ALL WIRES`.
- Switch rounds have no strike condition; only the clock pushes you.
- The switch hold is **500 ms**; a pattern that is correct but still moving does
  not count.

Report any mismatch against these four lines as a bug, because they are
intentional and the printed manual already tells players about them.
