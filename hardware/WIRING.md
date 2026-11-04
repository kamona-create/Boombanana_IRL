# BOMBANANA IRL — Wiring Specification

> The schematic (`hardware/schematic.pdf`) is the visual rendering of this
> document. The PCB to build from is this document + the perfboard layout.
> Do not invent pin assignments that are not listed here.

## Power Tree

| Rail | Source | Loads |
|---|---|---|
| 5V (`VBUS`) | External power bank → USB-C panel-mount extension (red wire) | ESP32 5V / VIN pin |
| 3V3 | ESP32 onboard regulator (output of the DevKit, `3V3` pin) | TFT VCC / RST / BL · 4×10k pull-ups |
| GND | ESP32 GND pin (also the other side of the USB-C extension) | everything |

## Banana Jacks (GPIO 13/14/16/17) — INPUT_PULLUP, LOW = wire plugged

Each jack: **side A → series 1 kΩ → node → GPIO. 100 nF from the SAME node → GND.**
All black-jack side Bs share one GND net.

| Signal | Series R | Cap (GPIO side → GND) | Node label |
|---|---|---|---|
| JACK1A | 1 k → GPIO13 | 100 nF | `JACK1_SIG` |
| JACK2A | 1 k → GPIO14 | 100 nF | `JACK2_SIG` |
| JACK3A | 1 k → GPIO16 | 100 nF | `JACK3_SIG` |
| JACK4A | 1 k → GPIO17 | 100 nF | `JACK4_SIG` |

Why: the jacks are exposed metal. The 1 kΩ limits the ESD current into the pin, and the 100 nF absorbs the pulse. Without both, one static shock can kill the ESP32 or make a phantom input for months.

## Switches (GPIO 34/35/36/39) — INPUT only, no internal pull-up

Per switch: **one terminal → GPIO, other terminal → GND, 10 kΩ from that GPIO → 3V3.**

| Signal | GPIO | Pull-up | Terminal |
|---|---|---|---|
| SW1 | 34 | 10 k → 3V3 | A contact only; COM → GND |
| SW2 | 35 | 10 k → 3V3 | A contact only; COM → GND |
| SW3 | 36 | 10 k → 3V3 | A contact only; COM → GND |
| SW4 | 39 | 10 k → 3V3 | A contact only; COM → GND |

On the perfboard: every GPIO34/35/36/39 **needs** the external 10 kΩ pull-up. Without it the pins float and the game reads random switch positions.

## Big Lamp — RGB common-cathode LED
| Segment | GPIO | Series R | Cathode |
|---|---|---|---|
| RED | 25 | 220 Ω | common → GND |
| GREEN | 26 | 220 Ω | common → GND |
| BLUE | 27 | 220 Ω | common → GND |

## Socket indicator LEDs — 5 mm green, active HIGH
| LED | GPIO | Series R | Cathode |
|---|---|---|---|
| SOCKET1 | 4 | 220 Ω | → GND |
| SOCKET2 | 21 | 220 Ω | → GND |
| SOCKET3 | 22 | 220 Ω | → GND |
| SOCKET4 | 12 | 220 Ω | → GND |

> GPIO12 is a strapping pin and must be LOW at boot. LED + 220 Ω → GND holds it LOW, which is correct. **Do not** wire these LEDs the other way (anode to 3V3, cathode to GPIO).

## Buzzer
| Side | Wired to |
|---|---|
| `+` | GPIO32 |
| `−` | GND |
| Module VDD | not used (GPIO-driven directly) |

## Start button (momentary push)
| Side | Wired to |
|---|---|
| one pin | GPIO33 |
| other pin | GND |

(GPIO33 is INPUT_PULLUP, so pressed = LOW.)

## TFT display (ILI9341, SPI)
| TFT pin | Wired to |
|---|---|
| VCC | 3V3 |
| GND | GND |
| BL | 3V3 |
| RST | 3V3 |
| CS | GPIO5 |
| DC | GPIO2 |
| MOSI | GPIO23 |
| MISO | GPIO19 |
| SCK | GPIO18 |

(TFT_eSPI is configured by `platformio.ini` via `build_flags`; RST is tied to 3V3 in hardware, so `TFT_RST=-1` in firmware.)

## USB-C panel-mount extension
| Connector role | Wired to |
|---|---|
| VBUS (5 V, red wire) | ESP32 VIN / 5V pin |
| GND (black wire) | ESP32 GND pin |
| CC pins | not used (extension cable handles them) |

## One-page ASCII net list

```
        +------------------ ESP32 -----------------+
VBUS ──►| 5V                                3V3 ├───► to TFT VCC/BL/RST, 4×10k
GND ───►| GND                                 │
        | GPIO13 ─[1k]──●─ JACK1A
        |               ├─[100nF]─ GND
        | GPIO14 ─[1k]──●─ JACK2A
        |               ├─[100nF]─ GND
        | GPIO16 ─[1k]──●─ JACK3A
        |               ├─[100nF]─ GND
        | GPIO17 ─[1k]──●─ JACK4A
        |               ├─[100nF]─ GND
        | GPIO34 ─[10k]─┤ 3V3
        |           └──●─ SW1-COM, SW1-A
        | GPIO35 ─[10k]─●─ SW2-COM, SW2-A
        | GPIO36 ─[10k]─●─ SW3-COM, SW3-A
        | GPIO39 ─[10k]─●─ SW4-COM, SW4-A
        | GPIO25 ─[220R]─ LAMP_R (common cathode → GND)
        | GPIO26 ─[220R]─ LAMP_G
        | GPIO27 ─[220R]─ LAMP_B
        | GPIO4  ─[220R]─ SOCK1  → GND
        | GPIO21 ─[220R]─ SOCK2  → GND
        | GPIO22 ─[220R]─ SOCK3  → GND
        | GPIO12 ─[220R]─ SOCK4  → GND
        | GPIO32 ─► BUZZER (+)
        | GPIO33 ─► START (+)
        | GPIO5  ─► TFT CS       GPIO23 ─► TFT MOSI
        | GPIO2  ─► TFT DC       GPIO19 ─► TFT MISO
        |                         GPIO18 ─► TFT SCK
        +----------------------------------------+
```

## Doing it in KiCad (the checklist)
1. Open/make project in `hardware/kicad/boombanana/`
2. Place symbols listed above (connector + Device + Switch libraries)
3. Assign values (`1k`, `10k`, `220R`) with right-click → Properties, or `E`
4. Draw wires `W`, power symbols `P` (+3V3/GND), **one PWR_FLAG on each of 3V3 and GND**
5. `Tools → Electrical Rules Check → Run ERC` — aim for 0 errors
6. `File → Page Settings` — title BOMBANANA IRL + date
7. `File → Plot` — PDF into `hardware/schematic.pdf`
8. Commit + push; attach the PDF to the submission

## Sanity checks before you ship the design
- [ ] Every jack has a 1 kΩ + 100 nF pair
- [ ] Every switch has a 10 kΩ pull-up on its GPIO (34/35/36/39)
- [ ] Every LED has its 220 Ω in series
- [ ] No net left floating (check ERC list)
- [ ] Every pin listed in this document appears somewhere in your schematic
