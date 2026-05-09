# Floorplanner

A wirelength-driven fixed-outline floorplanning tool for VLSI/EDA design.  
It uses **B\*-tree** as the floorplan representation and **Simulated Annealing (SA)** as the optimization engine to minimize chip area and wirelength while satisfying a target aspect ratio and whitespace budget.

---

## Algorithm Overview

### Floorplan Representation — B\*-tree

Modules are organized as a binary tree where:
- **Left child** = the module placed immediately to the right of the parent.
- **Right child** = the module placed immediately above the parent.

A contour-based **packing** algorithm traverses the tree to assign (x, y) coordinates to every module without overlap.

### Cost Function

The cost of a candidate floorplan is:

```
cost = α × Area + (1 − α) × (R_current − R*)²
```

Where:
- `Area` — bounding-box area of the packed layout.
- `R_current` — current aspect ratio (width / height) of the layout.
- `R*` — target aspect ratio (user-specified).
- `α` — a dynamic weight; adjusted during annealing based on how often recent solutions satisfy the fixed-outline constraints (H ≤ H\*, W ≤ W\*).

Wirelength is estimated by **Half-Perimeter Wirelength (HPWL)** across all nets.

### Perturbation Moves

Each SA iteration randomly applies one of three B\*-tree perturbation operations:

| Move | Probability | Description |
|---|---|---|
| **Node move** | ~25% | Detach a node and re-insert it as a left or right child of another node |
| **Node swap** | ~50% | Exchange two nodes in the tree |
| **Rotation** | ~25% | Rotate a module 90° (swap width ↔ height) |

### Annealing Schedule

- **Initial temperature:** 10⁸
- **Cooling rate:** T × 0.996 per iteration
- **Alpha adaptation:** α ∈ [0.5, 0.9], increases when feasible solutions appear more often
- **Termination:** when T < 10⁻⁵ and at least one feasible solution has been found

---

## Project Structure

```
.
├── src/
│   ├── main.cpp       # Entry point; argument parsing and top-level flow
│   └── parser.cpp     # All algorithm logic: parser, SA, packing, output
├── inc/
│   └── parser.h       # Class definitions (Pin, Module, Net, Node, B_tree, NETLIST)
├── makefile
├── ami33.yal          # Benchmark: 33 modules, 123 nets
├── ami49.yal          # Benchmark: 49 modules
├── apte.yal           # Benchmark: 9 modules
├── m/                 # Pre-generated MATLAB plot scripts
├── out/               # Pre-generated placement output files
└── txt/               # Pre-generated result summary files
```

---

## Build

Requires a C++11-compatible compiler (`g++` or `clang++`).

```bash
make
```

To clean and rebuild:

```bash
make clean && make
```

The executable `SA_N26124963` will be produced in the project root.

> **Note (macOS):** The original binary is a Linux ELF. Always recompile from source on macOS.

---

## Usage

```
./SA_N26124963 <input.yal> <aspect_ratio> <whitespace%> <output.txt> <output.out> <output.m>
```

| Argument | Description |
|---|---|
| `<input.yal>` | Input benchmark file in YAL format |
| `<aspect_ratio>` | Target aspect ratio R\* (e.g. `3` means height:width = 3) |
| `<whitespace%>` | Allowed whitespace percentage (e.g. `15` = 15% extra area) |
| `<output.txt>` | Result summary (area, ratio, wirelength, runtime) |
| `<output.out>` | Final placement: module name, x, y, rotation |
| `<output.m>` | MATLAB script for visualizing the floorplan |

### Examples

```bash
# ami33 benchmark, aspect ratio 3, 15% whitespace
./SA_N26124963 ami33.yal 3 15 ami33_3_15.txt ami33_3_15.out ami33_3_15.m

# ami49 benchmark, aspect ratio 1, 10% whitespace
./SA_N26124963 ami49.yal 1 10 ami49_1_10.txt ami49_1_10.out ami49_1_10.m

# apte benchmark, aspect ratio 2, 15% whitespace
./SA_N26124963 apte.yal 2 15 apte_2_15.txt apte_2_15.out apte_2_15.m
```

---

## Output Files

### `.out` — Placement result

One line per module:

```
<module_name>  <llx>  <lly>  <rotation>
```

- `llx`, `lly`: lower-left corner coordinates.
- `rotation`: `0` = original orientation, `1` = rotated 90°.

### `.txt` — Summary statistics

```
Benchmark: ami33.yal
ratio: <H/W>
whitespace: <%>
wirelength: <HPWL>
Chip_width: <W>
Chip_height: <H>
```

### `.m` — MATLAB visualization

Run in MATLAB or Octave:

```matlab
run('ami33_3_15.m')
```

This plots each module as a colored rectangle on the chip canvas.

---

## Benchmarks

| Benchmark | Modules | Nets |
|---|---|---|
| `ami33.yal` | 33 | 123 |
| `ami49.yal` | 49 | — |
| `apte.yal` | ~9 | — |

Pre-computed results for all combinations of aspect ratio (1, 2, 3) and whitespace (10%, 15%) are available in the `out/`, `txt/`, and `m/` directories.
