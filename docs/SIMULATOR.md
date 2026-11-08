# Simulator — `sim/index.html`

A single self-contained HTML file that runs the **same game logic** as
`src/main.cpp`: same 150 s timer, same 5-round plan (2 switch rounds), same
`TABLE_3` / `TABLE_4`, same 30 ms debounce and 500 ms switch hold. It is the
only way this firmware can be verified without hardware.

## Run it

Open `sim/index.html` in any modern browser (double-click is enough — no
server, no build, no network).

| Control | Action |
|---|---|
| **START** button / `Space` | start a game, or restart after BOOM / DEFUSED |
| Click a jack pair, or press `1` `2` `3` `4` | pull that wire |
| Click a switch, or press `Q` `W` `E` `R` | flip that switch |
| **إعادة كل الأسلاك** button | re-plug every wire (same as a player plugging them back) |
| **الصوت** button | mute / unmute the synthesised tones |

## What to verify (mirrors `hardware/TEST-PLAN.md` §2–4)

| # | Scenario | Expected |
|---|---|---|
| 1 | Start with all wires in | no `PLUG ALL WIRES`; round shows `PULL ONE WIRE` |
| 2 | Pull every wire, then re-plug all | round only arms again when **all four** are back |
| 3 | Pull the wrong wire | one `X`, round still advances |
| 4 | Pull the right wire | round advances, no `X` |
| 5 | Three wrong wires | `BOOM!` + red flash, no further input |
| 6 | Let the clock reach 0:00 | `BOOM!` |
| 7 | Clear all 5 rounds | `DEFUSED` + green flash |
| 8 | Play 3 games in a row | exactly 2 switch rounds each, order differs |
| 9 | Switch round: flip until `MATCH 4/4` | round passes after 500 ms of stillness |
| 10 | Switch round: keep flipping | the 500 ms hold restarts, no false pass |
| 11 | Switch round pattern | never all-UP and never all-DOWN |

A full playthrough takes about 4 minutes if you play it honestly (waiting for
the cable re-plug rule between rounds).

> A 3-wire round still has **four** physical jacks. The 4th socket is dead that
> round: its green LED is off and pulling it does nothing — it is not a strike.
> Same rule as firmware (`cut < wireCount`).

## What the simulator cannot prove

- TFT wiring, SPI pins, contrast and viewing angle
- The buzzer's real volume and tone (the browser synthesises its own)
- LED brightness / current, and the RGB lamp's true colour
- Debounce on **real** contacts — the sim models 30 ms, it does not measure it
- Power, ESD and connector reliability

Those are hardware checks and are listed as such in `hardware/TEST-PLAN.md`.
If a scenario above fails in the browser, it is a firmware bug, not a wiring
bug — fix it here first.
