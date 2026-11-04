# BOMBANANA IRL — Mechanical Dimensions (mm)

Everything here is in **millimetres**.

Every number is tagged with where it came from:

| Tag | Meaning |
|---|---|
| **MEASURED** | read out of `hardware/cad/case_shell_v1.stl` |
| **TARGET** | what the real part needs (section 3) — design to this |
| **SPEC** | a dimension chosen here so the case and the lid agree |

If a MEASURED number and a TARGET number disagree, the model is behind, not
this file. Section 1.3 lists the disagreements that still exist.

> Measured 4 Nov 2026 from `case_shell_v1.stl` (13 076 triangles).
> Re-export the STL and these numbers move — re-measure.

---

## 1. Case envelope — MEASURED

| Item | Value (mm) |
|---|---|
| Outer size | **153.04 (X) × 177.80 (Z) × 63.86 (depth, Y)** |
| X extent | −73.42 … +79.62 |
| Y extent | −60.96 (open mouth) … +2.90 (front face) |
| Z extent | −127.62 … +50.19 |
| Shape | one tub, open at **−Y**, front panel facing **+Y** |
| Internal volume | ≈ 1.38 L (141.27 × 169.93 × 57.60) |

The front panel lies in the **XZ plane**: Z runs "up" the panel and Y goes
*into* the case. That is how the model is built — do not confuse it with the
Onshape sketch planes when you redraw it.

### 1.1 Walls and cavity — MEASURED

| Feature | Value (mm) |
|---|---|
| Front panel thickness | 3.36 |
| Left wall (−X) | 5.53 |
| Right wall (+X) | 6.24 |
| Bottom wall (−Z) | 4.23 |
| Top wall (+Z) | 3.65 |
| Cavity X | −67.89 … +73.38 → **141.27 wide** |
| Cavity Z | −123.39 … +46.54 → **169.93 tall** |
| Cavity depth | **57.60** (−60.96 … −3.36) |
| Internal features | **none** — no ribs, no bosses, no standoffs |

The cavity is completely empty. Nothing in section 8 has a mount yet.

### 1.2 The mouth taper — the number that decides the lid

The outside of the shell is chamfered over the last ~6 mm before the mouth:

| Plane | Outer X | Outer Z |
|---|---|---|
| y = −55 (body) | 153.04 | 177.80 |
| y = −60.5 (mouth) | 148.23 | 175.40 |

**2.4 mm of inset per side.** So:

- The rim a lid lands on is only **2.7 mm (left), 3.8 mm (right), 4.2 mm
  (bottom), 1.2 mm (top)** wide. It is a seating face, nothing more.
- You **cannot** screw into that rim. The screws go into lugs added inside the
  cavity (§1.5).
- A lid matching the rim (148.23 × 175.40) is 4.8 mm smaller than the body's
  widest point, so there is a visible **2.4 mm step** at the seam. That is
  normal, and it doubles as a finger lip to open the lid. Do not "fix" it by
  making the lid 153.04 wide — it would overhang into thin air.

### 1.3 Front panel — MEASURED

Slice through the panel at y = −1.7. **19 openings**: a 4 × 4 grid of Ø8.00
holes, two extra Ø8.00 holes, and one 61.00 × 45.73 rectangle.

Coordinates are model (X, Z). **L** = from the left edge (X −73.42),
**B** = from the bottom edge (Z −127.62).

| Hole | X | Z | L | B |
|---|---|---|---|---|
| A1 | −32.76 | −109.99 | 40.7 | 17.6 |
| A2 | −32.76 | −83.10 | 40.7 | 44.5 |
| A3 | −32.86 | −55.55 | 40.6 | 72.1 |
| A4 | −32.86 | −27.61 | 40.6 | 100.0 |
| B1 | −7.37 | −109.98 | 66.1 | 17.6 |
| B2 | −7.36 | −83.01 | 66.1 | 44.6 |
| B3 | −7.46 | −55.55 | 66.0 | 72.1 |
| B4 | −7.46 | −27.61 | 66.0 | 100.0 |
| C1 | +17.94 | −109.98 | 91.4 | 17.6 |
| C2 | +17.94 | −82.92 | 91.4 | 44.7 |
| C3 | +17.94 | −55.55 | 91.4 | 72.1 |
| C4 | +17.94 | −27.61 | 91.4 | 100.0 |
| D1 | +43.34 | −109.98 | 116.8 | 17.6 |
| D2 | +43.35 | −82.83 | 116.8 | 44.8 |
| D3 | +43.35 | −55.55 | 116.8 | 72.1 |
| D4 | +43.34 | −27.61 | 116.8 | 100.0 |
| E1 | −53.06 | −95.46 | 20.4 | 32.2 |
| E2 | +61.64 | −41.81 | 135.1 | 85.8 |

| Screen window | X | Z | L | B | Size |
|---|---|---|---|---|---|
| centre | +3.17 | +12.09 | 76.6 | 139.7 | 61.00 × 45.73 |

Pitch in X is **25.40** (exactly one inch). Pitch in Z is **27.0 … 27.9** —
not uniform, so do not array it, place it.

### 1.4 Model vs parts — the gaps left

| Panel feature | Model has | Parts need (§4) | Fix |
|---|---|---|---|
| Banana jack | Ø8.00 × 18 | Ø8.20 × 8 | +0.2 for printing, delete 10 holes |
| Screen window | 61.00 × 45.73 | 48.9 × 36.7 glass (60 × 42.5 module) | too big — the glass edge shows |
| Toggle switch | — | Ø12.2 × 4 | missing |
| Start button | — | Ø12.2 × 1 | missing |
| LEDs (1 RGB + 4) | — | Ø5.1 × 5 | missing |
| Buzzer | — | Ø2.0 × 6 | missing |
| USB-C | — | 9.0 × 4.0 | missing |

The 18 × Ø8 grid is an inch-pitch drill pattern, not the final control layout.
Decide the layout first, then cut — a hole you cut is a hole you cannot move.

`hardware/cad/case_cover_v0_stub.stl` is a 12-triangle box, 142.13 × 2.5 ×
177.77, sitting at the **front** (y −2.5 … 0). It is not the lid: no holes, and
it does not match any face. Kept only so the file is not lost — delete it once
the real lid exists.

### 1.5 The lid — SPEC

Two parts, both flat, both print without supports.

**(a) Six lugs inside the shell** — add these, the cavity has none.

12 × 12 mm blocks, two faces flush with the cavity walls, running
y = −60.96 → −48.96 (12 mm deep into the case). Fillet R1.5 where they meet a
wall. Drill each **Ø4.0 × 9 deep** for an M3 heat-set insert (OD 4.0), the
insert centre 6 mm in from each cavity wall.

| Lug | X centre | Z centre |
|---|---|---|
| bottom-left | −61.89 | −117.39 |
| bottom-right | +67.38 | −117.39 |
| mid-left | −61.89 | −38.43 |
| mid-right | +67.38 | −38.43 |
| top-left | −61.89 | +40.54 |
| top-right | +67.38 | +40.54 |

Screw centres: **129.27 apart in X**, **78.97 / 78.96 in Z**.

**(b) The lid plate**

| Feature | Value (mm) |
|---|---|
| Outline | **148.23 (X) × 175.40 (Z)** |
| Outline centre | X +3.10, Z −39.92 |
| Thickness | 4.0 |
| Outer top edge | 1.0 × 45° chamfer |
| Screw holes | 6 × Ø3.4 through |
| Counterbore | Ø6.6 × 2.0 deep, outside face |
| Locating pins (optional) | 2 × Ø4.0 × 5.0 on two diagonal lugs, Ø4.2 holes in the lid |
| Assembled length | 63.86 + 4.0 = **67.86** |
| Fasteners | 6 × M3 × 12 button head |

The top rim is only 1.2 mm wide, so the **lugs** take the clamp load and the rim
is just a stop. That is why two of the six screws sit at Z +40.54, right at the
top edge.

**If you want it dust-tight:** add a spigot 140.67 × 169.33 × 4.0 (cavity −0.3
per side) with a 1.0 × 45° lead-in, centred on X +2.75 / Z −38.43. Note it is
**not** concentric with the lid outline — the walls are asymmetric. It must be
**notched at the six lugs** or it will not go in.

## 2. Lid joint — SPEC

The case is a **tub plus a lid**, not two equal shells. The tub prints open-side
up, the lid prints flat, and neither needs supports.

| Feature | Value |
|---|---|
| Seating face | the flat rim at y = −60.96 |
| Rim width | 2.7 (left) … 1.2 (top) — see §1.2 |
| Lid outline | 148.23 × 175.40, 4.0 thick |
| Clamp | 6 × M3 × 12 into heat-set inserts in six lugs |
| Insert | M3, OD Ø4.0, hole Ø4.0 × 9.0 deep |
| Lid clearance hole | Ø3.4 |
| Head counterbore | Ø6.6 × 2.0 deep |
| Locating | 2 × Ø4.0 × 5.0 pins on diagonal lugs, Ø4.2 in the lid |
| Gap between lid and rim | 0 — it sits flat; no gasket in v1 |

Notes:
- The lugs carry the load, the rim only stops the lid. Never put a screw through
  the 1.2 mm top rim.
- Every lug needs a R1.5 fillet at its base or it will snap off when the screw
  is tightened.
- The lid is opened every time you swap the power bank, so it gets inserts, not
  self-tapping threads.

## 3. Part dimensions (what you are fitting inside)

| Part | Dimensions (mm) | Mounting |
|---|---|---|
| ESP32 DevKit V1 (30-pin, Type-C) | PCB 51.5 x 25.5, height with USB ~12 | 2x M3 screws through a printed cradle, or a snap-in frame (board has no reliable holes) |
| 2.4" ILI9341 SPI module (no touch) | PCB 60.0 x 42.5, active area 48.96 x 36.72, thickness ~2.5 (glass+PCB) | 4x Ø2.5 holes, spacing ~55.5 x 37.5 — **verify** |
| Banana jack JS-910B | panel hole Ø8.0, cap Ø17, body behind panel 11–13, total length 33 | nut + washer behind the panel |
| 4 mm banana plug (male, solder) | pin Ø4.0, total length 38, body Ø9.5 | plugs into the jack |
| Toggle switch **MTS-203** (mini, 6-pin DPDT) | bushing **M6**, panel hole **Ø6.2**, body behind panel ~13, lever ~11 | nut + anti-rotation washer — **verify with calipers** |
| ~~Push button 12 mm (start)~~ | **not in `BOM.csv` yet** — the firmware uses `PIN_START` 33 | needs a part before the panel can be cut |
| 5 mm LED (1x RGB + 4x green) | body Ø5.0, height 6.0, legs on 2.54 pitch | press-fit into a Ø5.1 hole |
| Passive buzzer module 5V | ~20 x 15 x 8 (the BOM part is the 3-pin PCB type) | 2x M2, or a printed ring |
| Perfboard **8 x 12 cm** | 120.0 x 80.0 x 1.6, holes Ø1.0 on 2.54 pitch | 4x M3 holes at the corners, standoffs 6 mm |
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
| 4 | Start button | Ø12.2 | 1 | front | **blocked: no part in the BOM** — pick one first |
| 5 | Toggle switches | Ø6.2 + 1.5 keyway | 4 | front | MTS-203 mini toggle; 25 mm between centres. Swap to Ø12.2 only if you buy full-size M12 toggles |
| 6 | LED holes (1 RGB + 4 green) | Ø5.1 | 5 | front | 1 big lamp + 4 socket indicators; leave 1.2 mm between LED holes |
| 7 | Buzzer grille | Ø2.0 | 6 | front | drill pattern instead of one big hole, keeps dust out |
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
| Supports | none — tub prints open-side up, lid prints flat |
| Orientation | tub open-side **up**; lid outer face **down** |
| Print time | ~7–10 h for tub + lid, ~260 g |

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
printing, or the nozzle smears it. The front panel is the **bottom** of the tub
when it prints open-side up, so front-panel text has to be engraved on the
*inside* face instead — or the tub printed front-face-up on a raft, which is
worse. In practice: put player-facing text on the **lid**, and put numbers next
to the holes as printed labels or a sticker.

How to read the result: engraved grooves are the same colour as the case, so
fill them with a contrasting colour after printing:

1. Paint the engraved area with black acrylic / a matte paint pen.
2. Wait until tacky-dry, then wipe the flat surface with a cloth + a drop of
   IPA. The paint stays only inside the grooves.
3. One thin clear coat on top if it rubs off.

Onshape steps: open the tub → **Sketch** on the face you will print on top (the
lid's outer face, or the tub's front panel if you accept the trade-off) →
**Text** tool → type → set height in mm → place → green check →
**Extrude → Remove → 0.6 mm**. Text on a curved surface is not supported well —
keep all text on flat faces.

What is worth labelling (the players need it, the reviewer likes it):

| Where | Text |
|---|---|
| Lid, top centre | `BOMBANANA` (or `BOMBANANA IRL`) |
| Tub, under the screen | module name: `CABLE` / `SWITCH` |
| Tub, next to each jack | `1 2 3 4` — the manual refers to wires by number |
| Tub, next to the start button | `START` |
| Lid, bottom corner | `v1 · <your name> · 2026` — cheap, and it makes it yours |

## 11. Assembly order (matters)

1. Print and check the test plate.
2. Solder the perfboard and test everything on the bench **before** it goes in.
3. Heat-set the six inserts into the lugs (soldering iron, ~200 °C, straight in).
4. Mount jacks, switches, LEDs and the screen into the **tub**.
5. Screw the ESP32 and perfboard onto their standoffs inside the tub.
6. Plug the JST connectors, drop the lid on, fit the 6 × M3 screws — finger
   tight, then a quarter turn. Do not crush the rim.
7. Power on with the case open once, then again closed.

---

*Last updated: 4 Nov 2026*
