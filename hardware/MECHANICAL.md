# BOMBANANA IRL — Mechanical Dimensions (mm)

Everything here is in **millimetres**. There are no inches in this file.

Every number is tagged with where it came from:

| Tag | Meaning |
|---|---|
| **MEASURED** | read out of `hardware/cad/final.stl` |
| **TARGET** | what the bought part needs, from its datasheet or a caliper |
| **GAP** | MEASURED and TARGET disagree — listed in §6 |

> Measured 8 Nov 2026 from `hardware/cad/final.stl` (24 810 triangles,
> 1 240 584 bytes). Re-export the STL and these numbers move — re-measure.

The model is **one tub**, open at the mouth (rear). There is **no lid and no
internal mount** in this file; see §5 and `MOUNTS.md`.

---

## 1. Envelope — MEASURED

| Item | Value (mm) |
|---|---|
| Outer size | **153.22 (X) × 187.41 (Z) × 65.50 (Y, depth)** |
| X extent | −83.08 … +70.14 |
| Y extent | −60.96 (mouth) … +4.54 (front bosses) |
| Z extent | −118.63 (bottom) … +68.78 (top) |
| Front panel plane | **y = 0**, the panel goes *into* the case in −Y |
| Cavity volume | ≈ 1.40 L (142.98 × 180.91 × 54.00) |
| Mounting | none — no lugs, no standoffs, no screw holes |

The front panel lies in the **XZ plane**: Z runs "up" the panel and Y goes
*into* the case. That is how the model is built — do not confuse it with the
Onshape sketch planes when you redraw it.

### 1.1 Walls and cavity — MEASURED

| Feature | Value (mm) |
|---|---|
| Front panel | **6.96** (y 0 … −6.96) |
| Left wall (−X) | 5.00 (x −82.84 … −77.84) |
| Right wall (+X) | 5.00 (x +65.14 … +70.14) |
| Top wall (+Z) | 5.00 (z +63.78 … +68.78) |
| Floor (−Z) | **1.50** (z −118.63 … −117.13) |
| Back wall | **none** — the mouth is open |
| Cavity X | −77.84 … +65.14 → **142.98 wide** |
| Cavity Z | −117.13 … +63.78 → **180.91 tall** |
| Cavity depth | **54.00** (y −6.96 … −60.96) |
| Internal features | **none** — no ribs, no bosses, no standoffs |

The left inner face is not perfectly vertical: it leans from x = −78.06 at the
front to x = −77.44 at the mouth (**0.62 mm** over 54 mm). Nothing mounts to
it, so it only matters if you add a part that touches it.

### 1.2 Front perimeter

| Plane | Width in X | Note |
|---|---|---|
| y = 0 (front face) | 142.98 | inset 5.12 per side |
| y = −5 | 153.22 | full body from here back |

The front edge is rounded back over **5 mm**: the outer skin of the face is
142.98 wide, the body is 153.22. So the front face is 5.12 mm smaller than the
body on each side — that is intentional, not a modelling error.

### 1.3 The mouth — no seating rim

At y = −60.95 the wall ends in a **knife edge 0.36 mm wide**. There is no flat
rim for a lid to land on, and no step, groove or lip.

Consequence: a lid for this tub cannot be a flat plate with a spigot pushed
against a rim — there is nothing to push against. Any lid has to grip the
outside of the body (a cap) or the print has to grow a rim first. This is the
main reason the lid is out of scope (§5).

---

## 2. Front face — hole table — MEASURED

All diameters were found by 0.01 mm bisection of the mesh at **y = −3.5**
(mid-panel), so they are the holes through the 6.96 mm panel, not the front
bosses of §3.

Coordinates are model (X, Z). **L** = from the left edge (X = −83.08),
**B** = from the bottom edge (Z = −118.63).

### 2.1 Banana jacks — 8 holes

| # | X | Z | L | B | Ø |
|---|---|---|---|---|---|
| J1 | −42.44 | −18.52 | 40.64 | 100.11 | 8.19 |
| J2 | −17.08 | −18.52 | 66.00 | 100.11 | 8.19 |
| J3 | +8.41 | −18.40 | 91.49 | 100.23 | **8.43** |
| J4 | +33.83 | −18.40 | 116.91 | 100.23 | 8.19 |
| J5 | −42.44 | −46.28 | 40.64 | 72.35 | 8.19 |
| J6 | −16.92 | −46.28 | 66.16 | 72.35 | 8.19 |
| J7 | +8.41 | −46.37 | 91.49 | 72.26 | 8.19 |
| J8 | +33.83 | −46.37 | 116.91 | 72.26 | 8.19 |

Pitch in X is **25.4 mm** (one inch), pitch in Z is **27.79 mm**.
J3 is 0.24 mm larger than the other seven (7 × 8.19 + 1 × 8.43).

### 2.2 Toggle switches — 4 holes

| # | X | Z | L | B | Ø |
|---|---|---|---|---|---|
| SW1 | −42.34 | −74.07 | 40.74 | 44.56 | 6.19 |
| SW2 | −16.91 | −73.98 | 66.17 | 44.65 | 6.20 |
| SW3 | +8.41 | −73.77 | 91.49 | 44.86 | 6.19 |
| SW4 | +33.83 | −73.67 | 116.91 | 44.96 | 6.19 |

The row drifts **0.40 mm** in Z from SW1 to SW4 — it was built by hand, not by
an array. Place each hole, do not pattern it.

### 2.3 Indicator LEDs — 4 holes

| # | X | Z | L | B | Ø |
|---|---|---|---|---|---|
| L1 | −42.34 | −99.46 | 40.74 | 19.17 | 5.10 |
| L2 | −16.92 | −99.46 | 66.16 | 19.17 | 5.10 |
| L3 | +8.41 | −99.46 | 91.49 | 19.17 | 5.10 |
| L4 | +33.84 | −99.46 | 116.92 | 19.17 | 5.10 |

### 2.4 Start button — 1 hole

| X | Z | L | B | Ø |
|---|---|---|---|---|
| −63.40 | −86.95 | 19.68 | 31.68 | **12.19** |

### 2.5 Screen window — 1 rectangle

| Centre X | Centre Z | L | B | Size |
|---|---|---|---|---|
| −6.42 | +37.215 | 76.66 | 155.85 | **48.95 × 36.70** |

Matches the ILI9341 **glass** (≈ 48.9 × 36.7). The module PCB behind it is
≈ 60 × 42.5 mm — the window is a glass window, not a PCB window.

### 2.6 Small holes — 6 × Ø2.00

| X | Z | L | B |
|---|---|---|---|
| −6.13 | −114.84 | 76.95 | 3.79 |
| −61.38 | −104.58 | 21.70 | 14.05 |
| +52.21 | −105.68 | 135.29 | 12.95 |
| −5.06 | +62.82 | 78.02 | 181.45 |
| −55.54 | +62.87 | 27.54 | 181.50 |
| +53.89 | +62.16 | 136.97 | 180.79 |

They are 2.00 mm through the panel. Three sit near the bottom edge, three near
the top edge. They were described earlier as a buzzer grille; the positions
are at the edges, not over a buzzer, so treat the purpose as **unconfirmed**.

### 2.7 USB-C — through the TOP wall — MEASURED

| Axis | Extent (mm) | Size |
|---|---|---|
| X | −10.29 … −1.29 | 9.00 |
| Y | −25.85 … −21.85 | 4.00 |
| Z | +63.78 … +68.78 | 5.00 (through the wall) |

Straight cut, **no chamfer and no keyway**. A panel-mount USB-C extension
needs a flat 9 × 4 opening; the connector body behind it lives in the cavity.

### 2.8 Not in the model

No Ø3.0 reset / BOOT holes, no keyways on the switch holes, no counterbores
(see §3 for the front bosses), no screw holes anywhere.

---

## 3. Front bosses — MEASURED

Every front opening sits in a raised ring on the outside of the panel. The
rings protrude **+Y** (out of the case), so the front face is no longer flat:
the outermost point of the whole part is y = +4.54.

| Feature | Qty | Boss OD | Boss bore | Height | Panel hole |
|---|---|---|---|---|---|
| Banana jacks | 8 | 16.0 | 12.0 | 4.54 | 8.19 (×7) / 8.43 |
| Toggle switches | 4 | 12.5 | 8.5 | 4.54 | 6.19 |
| Indicator LEDs | 4 | 12.5 | 8.5 | 4.54 | 5.10 |
| Start button | 1 | 18.0 | 14.5 | 4.45 | 12.19 |

Because the bore is larger than the panel hole, each opening steps at y = 0:

```
front  ── Ø16.0 / bore Ø12.0 ── 4.54 deep  ─┐
                                             ├─ step at y = 0
panel  ── Ø8.19 through 6.96 ───────────────┘
```

### 3.1 What that means for the parts

A nut can only recess into the boss if it fits the **bore**:

| Part | Boss bore | Thread length available |
|---|---|---|
| Banana jack (nut on boss face) | 12.0 | 6.96 + 4.54 = **11.50** |
| Toggle switch (nut on boss face) | 8.5 | 6.96 + 4.54 = **11.50** |
| Start button (nut on boss face) | 14.5 | 6.96 + 4.45 = **11.41** |
| Banana jack (nut inside boss) | 12.0 | nut must fit Ø12.0 |
| Toggle (nut inside boss) | 8.5 | nut must fit Ø8.5 |

So before printing, check the real nut across-corners: a standard M6 or M8 hex
nut is 11.5–14.4 mm across corners and will **not** enter Ø8.5 or Ø12.0 — it
sits on top of the boss and needs the full 11.5 mm of thread. Verify the
thread length of the purchased part; the jack and switch datasheets are the
only authority here.

---

## 4. Rear side of the panel — MEASURED

| Feature | State |
|---|---|
| Rear sleeves (tubes around holes) | **removed on 16 holes** |
| Start button | **still has one**: bore Ø12.19, OD ≈ 17.2, y −6.96 … −10.86 |
| Front pegs | 16 × (2.5 × 2.4) × **2.9 high**, on the outside face, y 0 … +2.9, X −45 … +35, Z −114.4 … −108.4 |

The Start sleeve is the only place where the hole is deeper than the panel
(6.96 → 10.86). Everything else is a clean 6.96 mm wall plus the front boss.

The 16 front pegs sit in one row near the bottom edge, in front of the face.
They are the lowest thing on the front side after the bosses (4.54), so a
front-face-down print rests on the 16 pegs + the bosses, not on the face.

---

## 5. Depth budget — MEASURED

Free depth from the back of the panel to the mouth:

| Layer | From y | To y | Thickness |
|---|---|---|---|
| Front panel | 0.00 | −6.96 | 6.96 |
| **free space** | −6.96 | −60.96 | **54.00** |

Nothing is inside the cavity yet, so the 54.00 mm is the budget for the TFT,
the ESP32, the perfboard, the jack bodies and the wiring. For reference, the
TFT module (~5 mm PCB + 3.5 mm glass) plus the ESP32 (~24 mm with headers)
plus wiring (~3 mm) plus the jack bodies (~20 mm) is already ~55 mm if the
parts are stacked in series — so they have to sit **side by side**, not one
behind the other. That layout belongs to the build project.

---

## 6. Gaps left in this model

| # | Gap | Consequence |
|---|---|---|
| 1 | No lid, no seating rim (§1.3) | the case cannot be closed as exported |
| 2 | No internal mounts: no board standoffs, no screen pocket, no jack block | nothing is fixed inside the tub |
| 3 | No counterbores, only raised front bosses | see §3.1 — nut/thread check |
| 4 | No keyways on the switch holes | a tabbed anti-rotation washer cannot sit flat |
| 5 | J3 is Ø8.43 instead of Ø8.19 | 0.24 mm of extra play on one jack |
| 6 | Switch row drifts 0.40 mm in Z | hand-placed, not an array |
| 7 | 16 front pegs stick out 2.9 mm | front-face-down print needs supports |
| 8 | No Ø3.0 reset/BOOT access | the board is reachable only by opening the mouth |
| 9 | 6 × Ø2.00 holes unconfirmed (§2.6) | purpose unknown |
| 10 | USB-C cut is straight, no chamfer | a panel-mount connector needs a flat 9 × 4 face |

Items 1–2 are the reason the physical build is a **separate project**: this
export is a printed tub with a front panel, not a finished enclosure.

---

## 7. Print notes (for whoever prints it)

- Units in `final.stl` are millimetres; the file is binary STL.
- Envelope 153.22 × 187.41 × 65.50 → fits a 220 × 220 bed.
- The mouth is open (no back wall) and the floor is 1.50 mm — a thin bottom.
- Front-face-down rests on 16 pegs (2.9) and 17 bosses (4.54): either support
  the face or print mouth-down and support the bosses.
- 5.00 mm walls and a 6.96 mm panel are stiff enough for PLA at 3 walls / 20 %.
- No threads, no heat-set inserts, no M3 holes exist — plan inserts or
  self-tapping screws in the build project.
