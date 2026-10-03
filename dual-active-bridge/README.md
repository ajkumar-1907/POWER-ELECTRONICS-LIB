# Dual Active Bridge (DAB): 3.3 kW Bidirectional DC-DC Converter

A **3.3 kW, 400 V ↔ 350 V bidirectional isolated DC-DC power stage** for EV battery charging with **V2G (vehicle-to-grid)** capability. The board has two SiC full bridges connected through an off-board high-frequency transformer and series inductor. Power flows in either direction, set by the phase shift between the two bridges.

> **B.Tech Major Project-I (2026-27)**, Dept. of Electrical Engineering, Delhi Technological University. Anuj Kumar (23EE039) and Ankush (23EE033).

> ⚠️ **DANGER: 400 V DC.** The DC-link and battery capacitors store lethal energy and stay charged after power is removed. Bring the board up step by step (see [How to Use](#how-to-use)) and always measure before touching.

![PCB 3D angled view](pcb-3d-angle.png)

![Schematic](schematic.png)

![PCB Layout](pcb-layout2.png)

![PCB Top and Bottom](pcb-layout.png)

### 3D views

| Top | Bottom |
|---|---|
| ![3D top](pcb-3d-top.png) | ![3D bottom](pcb-3d-bottom.png) |

![3D rear angled view](pcb-3d-angle-rear.png)

### Schematic sections

| Bridge 1 (primary, 400 V) | Bridge 2 (secondary, 350 V) |
|---|---|
| ![Bridge 1](schematic-bridge1.png) | ![Bridge 2](schematic-bridge2.png) |
| **Isolated gate drive** | **Isolated V / I sensing** |
| ![Gate drive](schematic-gate-drive.png) | ![Sensing](schematic-sensing.png) |

![Connectors, fuses, control header and logic supply](schematic-connectors.png)

### Layer views (4-layer)

| L1 Front copper | L4 Back copper |
|---|---|
| ![Front copper](pcb-front-copper.png) | ![Back copper](pcb-back-copper.png) |
| **L2 In1: V1+ / V2+ / GND planes** | **L3 In2: PGND1 / PGND2 / +5 V planes** |
| ![Inner 1](pcb-inner1-copper.png) | ![Inner 2](pcb-inner2-copper.png) |

## Overview

| Block | Parts | Function |
|---|---|---|
| **Bridge 1 (primary)** | Q1–Q4 C3M0060065K, C1 100 µF/450 V, C3/C4 film + C7/C8 1812 | 400 V DC bus full bridge: legs A and B |
| **Bridge 2 (secondary)** | Q5–Q8 C3M0060065K, C2 150 µF/450 V, C5/C6 film + C9/C10 1812 | 350 V battery-side full bridge: legs C and D |
| **Gate drive** | U1–U4 UCC21520DW, PS1–PS8 MGJ2D051505SC | One dual isolated driver per leg and one isolated +15 / −5 V supply per switch (Kelvin-source return) |
| **Voltage sensing** | R30–R41 dividers, U5/U6 AMC1311B, PS9/PS10 + U7/U8 | Reinforced-isolated V1 and V2 measurement for the controller ADC |
| **Current sensing** | CS1/CS2 LEM LTSR 25-NP | Battery current (I_BAT) and transformer current (I_XFMR) |
| **Protection** | F1/F2 20 A 500 VDC fuses, R46/R47 220 k bleeders | Input/output fusing and capacitor discharge |
| **Control interface** | J9 2×10 header, J10 aux 5 V, D9/D10 OR-ing, U9 3.3 V LDO | 8 PWM inputs, enable, sense outputs to a C2000 LaunchPad |
| **Power terminals** | J1–J8 M4 bolt pads | DC bus ±, battery ±, transformer a/b (primary) and c/d (secondary) |

The board is split into **three isolated domains**:
- **Primary** (PGND1, 400 V)
- **Secondary** (PGND2, 350 V)
- **Control** (GND, 3.3/5 V)

Only the isolators cross between domains. Each one sits over a **2 mm routed slot**, with **≥ 8 mm** creepage/clearance between domains.

## Working Principle

1. **Two square waves.** Each full bridge switches at **20 kHz** with 50 % duty, diagonal pairs together (Q1+Q4 / Q2+Q3 on the primary, Q5+Q8 / Q6+Q7 on the secondary). Bridge 1 puts ±V1 on the transformer primary, and Bridge 2 puts ±V2 on the secondary.
2. **Phase shift sets power.** The controller delays Bridge 2 relative to Bridge 1 by a phase angle φ. The voltage difference across the series inductance Lk drives the current that transfers power.
3. **Bidirectional flow.** If φ > 0 (Bridge 1 leads), power flows from the DC bus to the battery (charging, G2V). If φ < 0, power flows from the battery back to the bus (V2G). The hardware is identical in both directions.
4. **Soft switching.** With the turns ratio matched to the voltages (d = n·V2 / V1 ≈ 1), the inductor current commutates each switch's output capacitance during the dead time. This gives **zero-voltage switching (ZVS)** over most of the load range.
5. **Isolation.** The 8:7 transformer gives galvanic isolation between bus and battery. The control side is isolated from both through the drivers, the AMC1311s and the LEM sensors.

## J9: Control Header (2×10, to the controller)

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | PWM1 (Q1) | 2 | PWM2 (Q2) |
| 3 | PWM3 (Q3) | 4 | PWM4 (Q4) |
| 5 | PWM5 (Q5) | 6 | PWM6 (Q6) |
| 7 | PWM7 (Q7) | 8 | PWM8 (Q8) |
| 9 | GND | 10 | GND |
| 11 | DIS (low = enable) | 12 | reserved (fault) |
| 13 | V1 sense + | 14 | V1 sense − |
| 15 | V2 sense + | 16 | V2 sense − |
| 17 | I_BAT | 18 | I_XFMR |
| 19 | +5 V | 20 | GND |

PWMn drives Qn directly. The controller firmware creates the diagonal pairing and the phase shift. Each UCC21520 also adds a hardware dead-time floor of about **500 ns** (R_DT = 49.9 k). DIS is pulled up, so all drivers stay **off** until the controller pulls DIS low.

## Key Equations

**Power transfer (single phase shift):**
```
P = n·V1·V2·φ·(π − |φ|) / (2π²·fs·Lk)        φ in rad, n = N1/N2 = 8/7

P_max (φ = π/2) = n·V1·V2 / (8·fs·Lk)
               = (8/7)(400)(350) / (8 · 20 kHz · 291 µH) ≈ 3.44 kW
At 3.3 kW:  φ ≈ 72°  (≈ 10 µs of the 50 µs period)
```

**Voltage conversion ratio (ZVS condition):**
```
d = n·V2 / V1 = (8/7)(350) / 400 = 1.0     → ZVS on both bridges over a wide load range
```

**Dead time (UCC21520):**
```
t_DT ≈ 10 ns/kΩ × R_DT = 10 × 49.9 k ≈ 500 ns     (1 % of the 50 µs period)
```

**Voltage sense divider (per bus):**
```
V_sense = V_bus × 8.06 k / (5 × 402 k + 8.06 k)  → 500 V gives 2.0 V (AMC1311 full scale)
```

## Applications

- On-board / off-board EV chargers with **V2G / V2H** capability
- Battery energy storage interfaces (bidirectional DC bus ↔ battery)
- Isolated DC-DC stage after a PFC front end
- Teaching and research platform for DAB modulation (SPS, EPS, DPS, TPS)

## Specifications

| Parameter | Value |
|---|---|
| DC bus (V1) | 400 V DC |
| Battery (V2) | 350 V nominal (300 V min) |
| Rated power | 3.3 kW, bidirectional |
| Switching frequency | 20 kHz |
| Transformer | 8:7, off-board (ETD59 / E65 core) |
| Series inductance Lk | 291 µH, off-board (or transformer leakage) |
| Switches | 8 × Wolfspeed C3M0060065K SiC, 650 V / 60 mΩ, TO-247-4 Kelvin source |
| Gate drive | +15 V / −5 V, 10 Ω on / 4.7 Ω off, 500 ns dead time |
| Isolation | 3 domains, ≥ 8 mm creepage, 2 mm slots under every isolator |
| Control | 3.3 V logic, C2000 LaunchPad (or any MCU with 8 PWM outputs) |
| Board | 220 × 150 mm, **4-layer**, 1.6 mm FR-4, **2 oz copper on all layers**, 6 mounting/heatsink holes |

## Bill of Materials (key parts)

| Ref | Value | Package |
|---|---|---|
| Q1–Q8 | Wolfspeed C3M0060065K (650 V, 60 mΩ SiC) | TO-247-4 |
| U1–U4 | TI UCC21520DW dual isolated gate driver | SOIC-16W |
| PS1–PS10 | Murata MGJ2D051505SC (5 V → +15 / −5 V, 5.2 kV) | SIP (THT) |
| U5, U6 | TI AMC1311BDWV reinforced isolated amplifier | SOIC-8 wide |
| CS1, CS2 | LEM LTSR 25-NP closed-loop current sensor | THT |
| C1 | 100 µF 450 V (Nichicon LGN2W101MELZ25) | Snap-in Ø25 |
| C2 | 150 µF 450 V (Nichicon LGN2W151MELZ30) | Snap-in Ø25 |
| C3–C6 | 1 µF 630 V polypropylene film (WIMA MKS4) | P22.5 mm |
| C7–C10 | 100 nF 1 kV X7R (KEMET C1812C104KDRAC) | 1812 |
| D1–D8 | BAT54J turn-off diode | SOD-323F |
| F1, F2 | 20 A 500 VDC fuse (Littelfuse 505) + clips | 6.3 × 32 mm |
| R46, R47 | 220 k 2 W bleeder | Axial |
| U7, U8 / U9 | 78L05 / AP2112K-3.3 | SOT-89 / SOT-23-5 |
| J9 / J10 | 2×10 2.54 mm header / Phoenix MKDS 2-way 5.08 mm | THT |
| J1–J8 | M4 ring-lug bolt pads | Ø4.3 mm plated |

The full BOM is 174 parts. 0805/1206 passives and their values are shown in the schematic.

## Design Notes

- **Off-board magnetics:** the transformer (8:7) and Lk (291 µH) connect through the M4 bolt pads: a/b to the primary, c/d to the secondary. Size the wires for the RMS current at 3.3 kW.
- **Kelvin source:** every MOSFET has its own isolated supply, referenced to its own Kelvin pin, so 8 supplies in total. Sharing a supply between low-side switches would tie the Kelvin pins together and defeat the Kelvin connection.
- **Commutation loop:** each leg has a film capacitor (top) and a 1 kV 1812 (bottom) placed directly at the MOSFET drain/source pins. V+ and PGND are laminated on In1/In2 to keep loop inductance low.
- **Isolation rules:** the custom DRC file (`dual-active-bridge.kicad_dru`) enforces:
  - 3 mm between different potentials inside a power domain;
  - 8 mm between domains;
  - 2 mm from HV copper to edges and slots.
  
  Netclasses are named `<domain>_<kind>_<potential>`.
- **Gate supply voltage:** the MGJ2D051505SC gives +15 / −5 V. For the recommended −4 V turn-off of the C3M series, use a −4 V variant or add a zener.
- **J9 pin 12** is reserved. The UCC21520 has no fault output, so the pin is free for a future over-current comparator.
- **Aux 5 V (J10):** the 10 isolated supplies draw about 0.5 A, which is more than a LaunchPad's 5 V pin can supply. J10 and J9 +5 V are OR-ed through D9/D10.
- **Heatsinks:** Q1–Q4 and Q5–Q8 mount on two separate heatsinks (H5/H6 mounting holes), one per isolated domain. Use insulating pads.
- **Before ordering:** check the footprints against their datasheets, especially:
  - the TO-247-4 pin order (1 D, 2 S, 3 Kelvin S, 4 G);
  - the AMC1311 DWV pin-out;
  - the fuse clips.

## Pros & Cons

| Pros | Cons |
|---|---|
| Bidirectional with identical hardware (G2V and V2G) | 8 active switches, 8 isolated gate supplies |
| Galvanic isolation and ZVS at d ≈ 1, giving high efficiency | ZVS is lost at light load or when d ≠ 1 (needs EPS/DPS/TPS modulation) |
| Simple single-phase-shift control | High circulating (reactive) current at light load |
| SiC + Kelvin source: fast, clean switching | Off-board magnetics must be designed and wound separately |
| Reinforced-isolated sensing, ready for closed-loop control | 4-layer, 2 oz board: higher fab cost |

## Files in This Folder

| File | Description |
|---|---|
| `dual-active-bridge.kicad_pro` / `.kicad_sch` / `.kicad_pcb` | KiCad 10 project, root schematic and PCB |
| `Bridge1` / `Bridge2` / `GateDrive` / `Sensing` / `Connectors.kicad_sch` | Hierarchical sub-sheets |
| `dual-active-bridge.kicad_dru` | Custom HV / isolation design rules |
| `DAB_3k3W.kicad_sym` / `DAB_3k3W.pretty/` | Project symbol library and fuse-clip footprint (loaded through `sym-lib-table` / `fp-lib-table`) |
| `dual-active-bridge-PTH.drl` / `dual-active-bridge-NPTH.drl` | Drill files |
| `gerbers/` | Gerber set for fabrication: 4 copper layers, masks, paste, silkscreen, Edge.Cuts with isolation slots, job file |
| `schematic.pdf` | Full 6-sheet schematic |
| `schematic.png` / `schematic-*.png` | Root sheet and section images |
| `pcb-3d-top.png` / `pcb-3d-bottom.png` / `pcb-3d-angle.png` / `pcb-3d-angle-rear.png` | 3D renders |
| `pcb-layout2.png` / `pcb-layout.png` | Layout previews |
| `pcb-front-copper.png` / `pcb-back-copper.png` / `pcb-inner1-copper.png` / `pcb-inner2-copper.png` | Copper layer views |

## How to Use

1. Open `dual-active-bridge.kicad_pro` in [KiCad](https://www.kicad.org/) 10 or later.
2. **To manufacture:**
   - Zip `gerbers/` together with the two `.drl` files and upload the zip.
   - Order **4 layers, 1.6 mm, 2 oz copper on all layers**.
   - In the order notes, mention that the board has **internal routed slots**. They are drawn on Edge.Cuts.
3. **Assemble:** the 1812 capacitors C7–C10 are on the bottom side. Mount Q1–Q8 on insulated heatsinks.
4. **Bring up in stages:**
   1. 5 V only: check every gate waveform with DIS low and no HV applied.
   2. 24–48 V on V1, current-limited, with a resistive load on V2.
   3. Step up to 400 V under supervision, behind a shield, with the fuses fitted.
5. **Close the loop:** read V1, V2, I_BAT and I_XFMR on the controller's ADC and regulate power with the phase shift φ.

## License

Released under the repository's [MIT License](../LICENSE).
