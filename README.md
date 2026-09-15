# puzzle-engine

A C++ engine that generates logic puzzles with a guaranteed unique, no-guessing solution.

> **Status:** early development. Build and usage instructions will be added once the
> first generator runs.

---

## Goals

- **Unique solution.** Every generated puzzle has exactly one valid completion.
- **No guessing.** The solution is reachable by pure deduction — a solver never has
  to make a speculative move and backtrack.
- **Graded difficulty.** Difficulty reflects the depth of reasoning a puzzle
  demands, not how many cells are blank.
- **Reproducible.** The same seed produces the same puzzle.
- **Extensible.** Puzzle types plug into shared generation and solving machinery.

### What "no-guessing" means here

A puzzle can have exactly one solution and still be unreachable without trial and
error. Uniqueness is a weaker property than solvability by deduction, and
this engine targets the stronger one: a puzzle is only accepted if it can be solved
by deduction alone.

---

## Puzzles

### Tango

A 6×6 grid filled with two symbols (☀ / ☾) under four rules:

- Each row and each column contains exactly three of each symbol.
- No three identical symbols may be adjacent, horizontally or vertically.
- Cells joined by `=` must hold the same symbol.
- Cells joined by `×` must hold different symbols.

A puzzle ships with a few pre-filled cells plus a set of `=` / `×` constraints,
chosen so that exactly one completion exists.

### Planned

- Minesweeper
- Mosaic

---

## Roadmap

- [ ] Tango grid representation and rule checking
- [ ] Deduction-based solver
- [ ] Generator producing unique, no-guessing puzzles
- [ ] Difficulty grading
- [ ] Command-line interface
- [ ] Additional puzzle types
