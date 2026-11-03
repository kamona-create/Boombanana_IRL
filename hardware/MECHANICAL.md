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
| 5 mm LED (1x RGB + 4x green) | body Ø5.0, height 6.0, legs on 2.54 pitch | press-fit into a Ø5.1 hole |
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
| 6 | LED holes (1 RGB + 4 green) | Ø5.1 | 5 | front | 1 big lamp + 4 socket indicators; leave 1.2 mm between LED holes |
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

## 8. Internal mounts — yes, every part needs one

Forge requirement, word for word: *"Have a concrete way to attach components
(including electronics). Use screws, clips, etc. This is a product, not a demo,
it should feel solid, and not held up by tape, glue, and dreams."*

So: no floating parts. But not every part needs a printed bracket — parts that
clamp through the panel (jacks, switches, button) are already mounted by their
own nut. The table below says which is which.

| Part | Mount | Numbers (mm) | Why |
|---|---|---|---|
| Screen | 4 bosses + M2.5 screws | boss OD Ø5.0, height 2.5, pilot Ø2.1 | it is the front face — a loose screen kills the product |
| ESP32 | printed cradle (3 walls) + 1 snap tab | wall 1.6 thick, 3.0 tall, board clearance +0.3 per side, tab 3.0 long x 1.2 thick | DevKit V1 has no trustworthy mounting holes |
| Perfboard | 4 standoffs + M3 self-tap | standoff OD Ø6.0, height 6.0, pilot Ø2.5 | keeps solder joints off the floor of the case |
| Banana jack | the part's own nut | hole Ø8.2, washer behind | the hole *is* the mount |
| Toggle switch | own nut + anti-rotation flat | hole Ø12.2 + 1.5 wide keyway slot | M12 switches spin if you don't key them |
| Start button | own nut | hole Ø12.2 | same |
| 5 mm LEDs (5) | press-fit + 0.6 mm retention lip | hole Ø5.1, lip on the inside | glue alone pops out with heat |
| Buzzer | two-piece ring, 2x M2 | ring OD Ø18, ID Ø12.2, ears with Ø2.2 | needs to be serviceable, not glued |
| Power bank | strap slots for velcro | slot 3.2 x 1.6, **do not** model a tight pocket | power bank sizes vary by ±5 mm |
| Wiring | 3x cable clips or zip-tie slots | clip gap 0.8 for 22 AWG, slot 3.2 x 1.6 | so pulling a cable never loads a solder joint |

Rules that keep mounts from breaking:

- Any boss touching a wall needs a R1.5 fillet at the base.
- Minimum 2 perimeters around every screw hole (that is why the boss OD is 7.0
  for an M3 insert).
- Snap tabs: 1.2 thick max in PLA, and they only survive ~20 cycles — use them
  for the ESP32 (rarely removed), not for the case lid.
- Anything opened more than twice gets a heat-set insert, not a self-tap thread.

## 9. What NOT to do

- ❌ Double-sided tape as the primary attachment (reviewers reject "held by tape").
- ❌ Glue for anything you might need to replace (buzzer, power bank, screen).
- ❌ A tight pocket for the power bank — you will change power banks.
- ❌ Mounts that block the USB port or the BOOT button.
- ❌ Screwing into a 3 mm wall without a boss.

## 10. Text on the case

Numbers that survive FDM printing (0.4 mm nozzle):

| Setting | Value |
|---|---|
| Method | **engrave** (extrude cut), not raised — raised text gets ironed flat and looks mushy |
| Engrave depth | 0.6 (0.8 if you use a 0.6 mm nozzle) |
| Minimum stroke width | 0.8 — below that the letters print as blobs |
| Minimum cap height | 6.0 (8.0 is comfortable) |
| Letter spacing | +5 % over default |
| Distance from any edge | ≥ 3.0 |
| Distance from any hole | ≥ 2.0 |
| Font | bold sans-serif, no serifs, no script |

**Print orientation trap:** engraved text must be on the **top** face while
printing, or the nozzle smears it. So the front shell prints **open side down,
front face up**. That means the rabbet on the rim becomes a 1.5 mm overhang —
give it a 45° chamfer on the inside edge and it prints without supports.

How to read the result: engraved grooves are the same colour as the case, so
fill them with a contrasting colour after printing:

1. Paint the engraved area with black acrylic / a matte paint pen.
2. Wait until tacky-dry, then wipe the flat surface with a cloth + a drop of
   IPA. The paint stays only inside the grooves.
3. One thin clear coat on top if it rubs off.

Onshape steps: open the front shell → **Sketch** on the front face → **Text**
tool → type → set height in mm → place → green check → **Extrude → Remove →
0.6 mm**. Text on a curved surface is not supported well — keep all text on
flat faces.

What is worth labelling (the players need it, the reviewer likes it):

| Where | Text |
|---|---|
| Front, top centre | `BOMBANANA` (or `BOMBANANA IRL`) |
| Under the screen | module name: `CABLE` / `SWITCH` |
| Next to each jack | `1 2 3 4` — the manual refers to wires by number |
| Next to the start button | `START` |
| Back, bottom corner | `v1 · <your name> · 2026` — cheap, and it makes it yours |

## 11. Assembly order (matters)

1. Print and check the test plate.
2. Solder the perfboard and test everything on the bench **before** it goes in.
3. Mount jacks, switches, LEDs and the screen into the **front** shell.
4. Screw the ESP32 and perfboard onto their standoffs in the **back** shell.
5. Plug the JST connectors, close the two halves, fit the 4 screws.
6. Power on with the case open once, then again closed.

---

*Last updated: 3 Nov 2026*
