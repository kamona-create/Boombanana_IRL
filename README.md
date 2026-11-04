# BOMBANANA IRL

A physical, three-player version of the bomb-defusal mini-game from
**BOMBANANA!** — one player is blindfolded, one is deaf, one cannot speak, and
the bomb only goes quiet if all three of them work together.

![Paper prototype](images/paper-prototype.jpg)
![Case design](images/case-design.png)

> Fan project. **BOMBANANA!** is by Lefto Studio — this is not affiliated with
> them, and nothing here is copied from their assets.

---

## Status

| Part | State |
|---|---|
| Game rules + paper prototype | ✅ done and playtested |
| Firmware: timer, strikes, buzzer, cable module | ✅ builds and runs |
| Firmware: switch module | 🔧 in progress |
| Case: shell (tub) | 🔧 modelled, being revised |
| Case: lid | 🔧 specced in [`hardware/MECHANICAL.md`](hardware/MECHANICAL.md) §1.5 |
| Hardware bought / printed | ⬜ not yet |

Firmware builds clean on `espressif32@6.12.0`: RAM 6.8 %, flash 23.5 %.

---

## How you play

Three roles, three handicaps, and nobody can see the whole problem:

| Player | Handicap | Can |
|---|---|---|
| 🙈 **Blind** | blindfolded | touch the bomb — the jacks, the switches, the raised dots |
| 🙉 **Deaf** | ear defenders | see the screen, the lamp colour, the ticking |
| 🙊 **Mute** | may not speak | read the manual, and only gesture |

The mute player reads the rule for this round, gestures it to the deaf player,
the deaf player describes it out loud to the blind player, and the blind player
cuts the right cable before the timer hits zero.

**3 strikes and it explodes.**

### The cable round

1. The big RGB lamp shows a colour — **red**, **green** or **blue**.
2. Three or four wires are plugged into the banana jacks.
3. The manual maps that colour + wire count to a position.
4. The blind player pulls one wire. Wrong wire = strike.

Only the mute player knows the mapping, and they can't say it. That is the
whole game.

---

## Hardware

| Part | Qty | Notes |
|---|---|---|
| ESP32 DevKit V1 (30-pin, USB-C) | 1 | one board runs everything |
| 2.4" ILI9341 SPI TFT (no touch) | 1 | timer + strikes |
| Banana jack JS-910B | 8 | 4 pairs, panel hole Ø8.0 |
| 4 mm banana plug | 8 | one per pullable wire |
| MTS-203 mini toggle | 4 | switch module |
| 5 mm LED | 5 | 1 common-cathode RGB + 4 green |
| Passive buzzer module | 1 | must be **passive** — active ones can't play tones |
| Perfboard 8 × 12 cm | 1 | all wiring lives here, no PCB in v1 |
| JST-XH connectors | — | modules plug in, nothing is soldered to the case |

Full bill of materials with prices and purchase links:
**[`BOM.csv`](BOM.csv)** — total **$152.87** including tools.

Every dimension, hole and tolerance:
**[`hardware/MECHANICAL.md`](hardware/MECHANICAL.md)**.

### Pin map

| Signal | GPIO |
|---|---|
| Jack 1 / 2 / 3 / 4 | 13 / 14 / 16 / 17 |
| Socket indicator LEDs ×4 | 4 / 21 / 22 / 12 |
| RGB lamp R / G / B | 25 / 26 / 27 |
| Buzzer | 32 |
| Start button | 33 |
| TFT CS / DC | 5 / 2 |
| TFT MOSI / MISO / SCK | 23 / 19 / 18 |
| TFT RST | none — tied to 3V3 |

`GPIO12` is a strapping pin and must be low at boot; it drives an LED through a
resistor to ground, which is exactly what it wants. `GPIO15` is deliberately
unused. Free pins for the switch module: 34 / 35 / 36 / 39 (input only, they
need external 10 kΩ pull-ups).

The jacks are read with `INPUT_PULLUP`: a plugged wire shorts the pin to ground.
Every input is debounced for 30 ms, and a pulled wire is latched once so that
grabbing two wires at the same time only counts as one cut.

---

## Building the firmware

Needs [PlatformIO](https://platformio.org/).

```bash
pio run            # build
pio run -t upload  # flash
```

On Windows you can also double-click `build.bat`, which retries if the USB
connection drops.

TFT_eSPI is configured entirely through `build_flags` in `platformio.ini` —
there is no edited `User_Setup.h` to forget about.

---

## Repo layout

```
src/main.cpp              firmware — game state machine, display, audio
platformio.ini            board + TFT_eSPI pin configuration
BOM.csv                   every part, price and link
hardware/MECHANICAL.md    measured dimensions, hole table, lid spec
hardware/MOUNTS.md        work order for the internal mounts
hardware/cad/             STL exports
JOURNAL.md                build log
PLAN.md                   working plan (Arabic)
```

---

## Future work

- Switch module firmware (4 toggles on the free input-only pins)
- Custom PCB — v2, the perfboard is a v1 compromise
- DFPlayer Mini for real voice lines and explosion audio
- Direction and Calculation modules, to match the full game
- A proper lid with a dust seal

---

## License

MIT — see [`LICENSE`](LICENSE).
