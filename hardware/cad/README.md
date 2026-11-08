# `hardware/cad/` — STL exports

All files are **binary STL**, units **millimetres**, axes as exported from
Onshape (Y is the case depth, Z is up the front panel).

| File | Size | Triangles | What it is |
|---|---|---|---|
| `final.stl` | 1 240 584 B | 24 810 | **The current case.** One tub: front panel, 5 mm walls, 1.5 mm floor, 17 front bosses, 16 front pegs. Open at the back. |
| `case_final.stl` | 684 B | 12 | **Placeholder, not a model.** A 12-triangle box left over from an earlier upload. Do not print it and do not measure it. |

`final.stl` is the authoritative geometry. Every dimension quoted in
[`hardware/MECHANICAL.md`](../MECHANICAL.md) was measured from it on
8 Nov 2026 (bisection of the mesh at fixed Y planes), and every number there
is tagged **MEASURED**, **TARGET** or **GAP**.

## What `final.stl` contains

- Front panel **6.96 mm** thick, walls **5.00 mm**, floor **1.50 mm**.
- Envelope **153.22 (X) × 187.41 (Z) × 65.50 (Y)**.
- 8 jack holes Ø8.19 (one is Ø8.43), 4 switch holes Ø6.19, 4 LED holes
  Ø5.10, 1 Start hole Ø12.19, screen window 48.95 × 36.70.
- 16 raised front bosses around the holes (OD 16.0 / 12.5 / 18.0) plus one
  around Start, all 4.45–4.54 mm proud of the face.
- 16 small front pegs, 2.5 × 2.4 × 2.9 mm, in a row near the bottom edge.
- One 9.00 × 4.00 USB-C opening through the top wall.
- A rear sleeve only behind the Start button (to y = −10.86).

## What `final.stl` does **not** contain

- No lid, no seating rim, no lugs, no screw holes.
- No board standoffs, no screen pocket, no clips, no cable guides.
- No counterbores, no switch keyways, no Ø3.0 reset/BOOT holes.

Those are deliberate: this file is the **tub**, and the lid plus internal
mounts belong to the separate build project (see
[`../MOUNTS.md`](../MOUNTS.md) and [`../TEST-PLAN.md`](../TEST-PLAN.md)).

## Printing

- Bed: the envelope fits a 220 × 220 mm bed with the mouth down.
- The mouth ends in a **knife edge 0.36 mm** wide — there is no flat rim.
- Front-face-down rests on the 16 pegs (2.9 mm) and the 17 bosses (4.54 mm),
  so either support the face or print mouth-down and support the bosses.
- PLA, 3 walls, 20 % infill is enough for 5 mm walls; the 1.5 mm floor is the
  weak point, so handle the tub by the walls.

Re-export from Onshape and all of these numbers move — re-measure before
printing. The measurement scripts used are outside the repo; the method is
described at the top of [`../MECHANICAL.md`](../MECHANICAL.md).
