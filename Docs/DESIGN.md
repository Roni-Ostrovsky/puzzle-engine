# Design decisions

This is a decision log, not an architecture description. Each entry records a choice, the
alternatives, the reasoning, and the condition that would make it worth revisiting.

Entries are append-only. When a decision is reversed, a new entry is added that references
the old one rather than editing it

---

## Open questions

Things not yet settled. Listed here so they are not mistaken for decisions.

- **Board state: assigned values, or candidate sets per cell?**
  A deduction solver reasons about what is still possible, not about what has been decided,
  which argues for candidate sets with a solved cell being the singleton case.
  Not yet committed; will be decided when the propagating solver is written and the requirement is concrete.
- **Does reasoning depth have usable range on a 6x6 Tango board?**
  The difficulty model assumes it does.
  It is possible that once enough clues exist to force uniqueness, nearly
  every board is depth 0 or depth 1 and the scale collapses.
- **What a violation reports to the caller?**
  currently its: rule + line/anchor cell, expected to change as solver needs become clearer.

  

---

## Decisions

### D1 - The core library performs no I/O

**Decided:** The engine neither reads files nor writes to streams.
Only the CLI and any later service layer touch the outside world.

**Alternatives considered:** Letting the solver print its own trace directly.

**Why:** A REST layer and a frontend are planned. Anything that prints from inside the
engine has to be untangled before either can be added.

**Would revisit if:** Nothing foreseeable.

**Date:** 2026-09-15

---

### D2 - One explicit RNG, threaded through every random choice

**Decided:** A single random source passed explicitly to anything that makes a random
decision.

**Alternatives considered:** A convenience global, which is shorter to write.

**Why:** The README promises that the same seed produces the same puzzle.

**Would revisit if:** Never for the seed guarantee itself.

**Date:** 2026-09-15

---

### D3 - Difficulty is graded by length of propegation to contrediction after hypothetical guess

**Decided:** To enforce the "No guessing rule" each guess will be capped by length of reasoning until contrediction.
This can be used as apractical difficulty grading.
Length '0' reasoning means no guess was made and its purly by constraints.

**Alternatives considered:** Writing an enumerated catalogue of named inference techniques
and grading by the most sophisticated technique required.

**Why:** Two reasons:
1. It's abstract, so the same definition serves Tango, Minesweeper and Mosaic.
  So the difficulty model can become shared rather than re-invented per puzzle.
2. It's straightforward and smoother difficulty grading, a list of human techniques has a hard limit on its size and length,
  eventually any new technique is highly dependant on the puzzle state and cannot be generelized practically.

**Trade-off accepted:** Named techniques would have given a human-meaningful difficulty
scale and a natural hint/explanation feature. Those can be layered on later for
presentation, they are no longer prerequisites.

**Would revisit if:** Generated difficulty turns out not to correlate with how hard puzzles
actually feel to solve by hand.

**Date:** 2026-09-18

---

### D4 - Build Tango specific engine and generelize later

**Decided:** building tango specific is much simpler at the start.

**Alternatives considered:** generalizing early for multiple puzzles types.

**Why:** It's much harder to generalize an unwriten code from the get-go.
  with proper classes and functions seperation the transition will be simple and not too long.

**Trade-off acceped:** waste about a day later on to generalzie it.

**Date:** 2026-09-22

---

### D5 - Tango not restricted to 6x6 grid

**Decided:** build tango with veriable size.

**Alternatives considered:** restricting to a 6x6.

**Why:** the restrictions on Tango arn't specified on the size.
  any posetive even size thats bigger than 6 will work just as well.

**Date:** 2026-09-23

---

### D6 - Test framework

**Decided:** doctest as the framework

**Alternatives considered:** Catch2 and GoogleTest

**Why:** It's a single header committed to the repo, so there's nothing to install, fetch or link.
  Catch2 v3 is no longer single-header, it's a compiled library.

**Would revisit if:** need mocking or if I use tools that expect GoogleTest output.

**Date:** 29.9.26

---

### D7 - Tango grid restricted between 6 and 16

**Alternatives considered:** not capping the size of the grid.

**why:** Tango relise on a small grid to be human friendly, i estimate that 16x16 will be much bigger than needed for general human use.

**Would revisit if:** my estimation prooves incorrect, or remove temporarily for stress-testing the engine.

**Date:** 29.9.26

---

### D8 - Constraints lookup by cell index

**Alternatives considered:** lookup by memory indices

**Why:** lookup by memory indices will require conversion from cell index to the constraint index (c-c/size).
  this way lookup is simpler and more consistent from outside the class and does no require outside acknowledgment of internal memory management.

**additional impact:** GetNeighborConstraints(i) will now return a "None" restraint type if theres no neighbor.

**Would revisit if:** some rules will need to tell "no neighbor" apart from no constraint.

**Date:** 29.9.26

---

### D9 - Checkers recive board as cost Board&

**Alterntives:** raw pointer or passing by value

**Why:** no copy, compiler enforces read-only

**Date:** 30.9.26

---

### D10 - Cells use flat indices

**Alternatives:** 2D coordinates accessing

**Why:** the index arithmatic is symple enough to handle directly.

**Date:** 30.9.26

---

### D11 - Tango checkers work per row/column

**Alternatives:** passing line of cells to check

**Why:** every Tango rule is on the same row/column
  passing the cells will push the knoledge of the rules to the callers.

**Date:** 30.9.26

---

### D12 - Checkers report the first violation

* checks will run in a fixed order (constraints, triplet, balance)

**Alternatives:** collecting every violation

**Why:** saves work, the solver only needs to know that there's a violation

**would revisit:** if hints/explanations need all violations.

**Date:** 30.9.26

---

### D13 - rule enum class a value per constraint type

**Alternatives:** single constraint value covering '=' and 'x'

**Why:** callers can tell exactly which rule failed

**Date:** 30.9.26

---

### D14 - rule checks on partial boards, empty = unknown

* A board passes when no filled cells create a violation
* A board is solved when it passes with no empty cells

**why:** passes the propegation fail-point to the engine and not an instent check.
  creates a more uniform checking for different parts and assists in **D3**.

**Date:** 30.9.26

---
