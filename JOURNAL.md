---
title: "BOMBANANA IRL"
author: "Monzer Taher"
description: "A 3-player physical bomb-defusal game inspired by BOMBANANA! on Steam, built on an ESP32."
created_at: "2026-11-02"
---


# November 2: Paper prototype and first plan

<!--
What to write (answer all three):
- WHAT: you played a paper version of the 3-player game and it worked.
- WHY: why did you test on paper before buying anything? What did playing it
  teach you about the rules (timer length, number of rounds, strikes)?
- PROBLEM: what broke or felt unfair in the paper version, and what did you change?
Also: which module did you drop for v1, and why?
-->

![paper prototype](images/paper-prototype.jpg)

**Total time spent: 2h 7m hours**

# November 3: Picking real parts and drawing the pin map

<!--
What to write:
- WHAT: researched real parts on ielectrony / dawaer / Amazon.sa and checked live prices.
- WHY: why ILI9341 without touch? why 8 banana jacks instead of 6? why JST-XH
  connectors between the modules instead of one big board? why perfboard and not a PCB
  in v1? why the JS-910B socket (Ø8 mm panel hole)?
- PROBLEM: the ESP32 pin map conflicts. VSPI needs GPIO18 for SCK and the buzzer was
  on GPIO18. Write down how you found it and what you moved.
- Screenshots to include: Wokwi diagram, the BOM spreadsheet, VS Code.
-->

![wokwi wiring diagram](images/diagram.png)

![BOM with live prices](images/bom.png)

**Total time spent: 3h 35mhours**
