# BOMBANANA IRL — Mechanical Dimensions (mm)

Everything here is in **millimetres**. All numbers are for the v1 case:
**200 x 140 x 45 mm**, 3 mm walls, two shells (front + back).

> These are the *reference* numbers taken from part datasheets and from the
> parts in the BOM. Your exact parts may differ by 0.2–1 mm — measure the real
> part with calipers before you commit a dimension. That is normal, and it is
> exactly why the test plate exists (section 6).

---

## 1. Case envelope

| Item | Value |
|---|---|
| Outer size | 200 x 140 x 45 |
| Wall thickness | 3.0 |
| Internal size | 194 x 134 x 39 |
| Corner radius (outside) | 6.0 |
| Split | front shell + back shell, split plane at 45 / 2 = 22.5 from the front face |

**Split line:** cutting it this way means the front face is one flat piece
(no layer lines across the screen bezel) and both halves print with the open
side up — zero supports.

## 2. Front/back joint (rabbet + screws)

| Feature | Value |
|---|---|
| Rabbet width (the step around the rim) | 6.0 |
| Rabbet depth | 1.5 |
| Clearance between the two shells at the step | 0.2 |
| Alignment pins | 2x Ø3.0 x 5.0, holes Ø3.2 x 5.5 |
| Screw bosses (in the back shell) | OD Ø7.0, length = 12 |
| Heat-set insert hole (front shell) | Ø4.0 x 6.0 deep (M3 insert, 4.0 OD) |
| Clearance hole (back shell) | Ø3.4 |
| Head counterbore | Ø6.6 x 1.8 deep |
| Screws | 4x M3 x 10, at the 4 corners, 8 mm in from each edge |

Notes:
- Pins go on the back shell, holes on the front shell. Pins take the shear load
  so the screws don't have to locate the two halves.
- The boss must be joined to the wall with a fillet (R1.5) or it will snap off.

## 3. Part dimensions (what you are fitting inside)

| Part | Dimensions (mm) | Mounting |
|---|---|---|
| ESP32 DevKit V1 (30-pin, Type-C) | PCB 51.5 x 25.5, height with USB ~12 | 2x M3 screws through a printed cradle, or a snap-in frame (board has no reliable holes) |
| 2.4" ILI9341 SPI module (no touch) | PCB 60.0 x 42.5, active area 48.96 x 36.72, thickness ~2.5 (glass+PCB) | 4x Ø2.5 holes, spacing ~55.5 x 37.5 — **verify** |
| Banana jack JS-910B | panel hole Ø8.0, cap Ø17, body behind panel 11–13, total length 33 | nut + washer behind the panel |
| 4 mm banana plug (male, solder) | pin Ø4.0, total length 38, body Ø9.5 | plugs into the jack |
| Panel toggle switch (SPDT, M12) | thread M12 x 0.75, panel hole Ø12.2, body behind panel ~20, lever 12 | nut behind the panel |
| Push button 12 mm (start) | panel hole Ø12.2, body behind panel ~18 | nut behind the panel |
| WS2812B 5 mm LED | body Ø5.0, height 6.0, 3 legs on 2.54 pitch | press-fit into a Ø5.1 hole, glued or friction |
| Passive buzzer (bare, 5V) | Ø12.0 x 9.5 | Ø12.2 hole + a drop of hot glue, or a printed ring |
| Buzzer module (3-pin PCB type) | ~20 x 15 x 8 | M2 screws or glue |
| Perfboard 15 x 9 cm | 150.0 x 90.0 x 1.6, holes Ø1.0 on 2.54 pitch | 4x M3 holes at the corners, standoffs 6 mm |
| JST-XH 2-pin | 2.50 pitch, housing ~7.4 x 4.5 | free-hanging (cable), no case feature |
| Power bank 5000 mAh | ~90 x 62 x 12 (varies!) | Velcro or a printed strap — do not model it as a tight pocket |
| USB-C plug shell | 8.4 x 2.6 | opening 9.0 x 4.0 with 1 mm chamfer on the outside |

Add **2 mm** of clearance around every part that is not screwed down
(glass edges, buzzer, power bank strap).

## 4. Cutout table (all holes in the case)

| # | Cutout | Size | Qty | Panel | Notes |
|---|---|---|---|---|---|
| 1 | Banana jack | Ø8.2 | 8 | front | 2 rows of 4, 20 mm between centres, ≥18 mm of flat around each |
| 2 | Screen window | 48.9 x 36.7 | 1 | front | centre it; allow 4 mm of bezel overlap on each side so the glass is covered |
| 3 | Screen screw bosses | Ø2.5 | 4 | front (inside) | bosses Ø5.0, height = 2.5 (module thickness) |
| 4 | Start button | Ø12.2 | 1 | front | 12 mm push button |
| 5 | Toggle switches | Ø12.2 | 4 | front | switch module panel, 25 mm between centres |
| 6 | WS2812B LEDs | Ø5.1 | 5 | front | 1 big lamp + 4 socket indicators; leave 1.2 mm between LED holes |
| 7 | Buzzer | Ø2.0 | 6 | side | drill pattern instead of one big hole, keeps dust out |
| 8 | USB port | 9.0 x 4.0 | 1 | right side | 25 mm from the front face so the cable doesn't foul the table |
| 9 | Reset/BOOT access | Ø3.0 | 2 | back | so you can reflash without opening the case |
| 10 | Module JST pass-through | Ø8.0 | 2 | internal wall | cable path between the two module areas |

## 5. FDM tolerances (print these, don't fight them)

| Feature | Value |
|---|---|
| Hole clearance (for a part you insert) | +0.2 on diameter |
| Press-fit hole (LED) | +0.1 on diameter |
| Sliding fit (shell step) | +0.2 on the step, per side |
| Minimum wall | 2.4 (use 3.0) |
| Minimum gap between two holes | 1.2 |
| Minimum distance from hole edge to a wall | 2.5 |
| Heat-set insert hole | 4.0 (for 4.0 OD insert) — test 1 first |
| Screw thread into plastic (no insert) | pilot Ø2.5 |

## 6. Test plate (do this before the full case)

One 40 x 40 x 3 plate with:

```
8.0   8.1   8.2   8.3      <- banana jack
4.0   4.1                  <- heat-set insert
```

Push the real jack into each hole, screw the M3 insert into 4.0 and 4.1, and
keep the size that feels right. Photograph it — this is a good journal entry
and it proves your tolerances are measured, not guessed.

## 7. Print settings that actually work for this

| Setting | Value |
|---|---|
| Material | PLA (PETG only if the case will sit in a car) |
| Layer height | 0.2 |
| Walls / perimeters | 3 |
| Infill | 20 % (gyroid) |
| Supports | none — that's the point of the split |
| Orientation | both shells open-side up, front face on the bed |
| Print time | ~6–9 h for both shells, ~250 g |

## 8. Assembly order (matters)

1. Print and check the test plate.
2. Solder the perfboard and test everything on the bench **before** it goes in.
3. Mount jacks, switches, LEDs and the screen into the **front** shell.
4. Screw the ESP32 and perfboard onto their standoffs in the **back** shell.
5. Plug the JST connectors, close the two halves, fit the 4 screws.
6. Power on with the case open once, then again closed.

---

*Last updated: 3 Nov 2026*
