# Miscellaneous

A grab-bag of one-off exercises not tied to a single course — small C++ programs, a couple of stray assembly labs, Prolog assignments, and fragments of game source. **Languages:** C++, x86-64 Assembly (NASM), MIPS Assembly, Prolog, Shell, CSS/PHP.

> The two Common Lisp labs that used to live here moved to their real course, [`../programming-languages/`](../programming-languages/) (CMPS 3500). A few files below (`lab6.cpp`, `lab12.cpp`) are earlier alternate versions of labs that also exist, more polished, under `../software-development/`.

| File | What it is |
|------|------------|
| `mla-citation-generator.cpp` | MLA citation generator: a `CiteGen` class prompts for author/title/publisher/year |
| `lcm.cpp` | Least common multiple by brute-force incrementing |
| `miles_per_gallon.cpp` | Prompts for gallons and miles, prints MPG |
| `randnum.cpp` | Prints a single random number 0–99 |
| `membershipfees.cpp` | Yearly-fee/interest loop exercise (intentionally buggy) |
| `integer-overflow-demo.cpp` | Counts up from `0xFFFFFFF` forever with `printf` to show integer overflow |
| `name-insult-joke.cpp` | Joke program: prints an insult based on the name entered |
| `perceptron-feedforward.cpp` | Perceptron feed-forward: weighted-sum vs. theta step function, one weight update |
| `perceptron-iris-training.cpp` | Single-layer perceptron trained on `iris-binary-normalized.csv` over 200 epochs |
| `median-of-two-sorted-arrays.cpp` | LeetCode: median of two sorted arrays via full merge (an alternate take on the one in `../../leetcode/`) |
| `lab6.cpp` | C++ debugging exercise: count dictionary words whose 4th letter is `a` (intentionally buggy) |
| `lab12.cpp` | C++ equal-temperament musical-note frequency calculator from A4 = 440 |
| `lab1.pl` | Prolog: forward-chaining item-derivation rules |
| `lab05.pl` | Prolog: animal-taxonomy facts/rules, max-of-3, Towers-of-Hanoi move count |
| `Soto_Erik_Lab09.s` | x86-64 NASM: reads two ints, prints the larger and their sum via printf/scanf |
| `Soto_Erik_Lab10.asm` | x86-64 NASM: loops reading numbers and accumulating their sum |
| `Makefile` | Assembles/links `Soto_Erik_Lab10.asm` via `nasm -f elf64` + gcc (CMPS 2240) |
| `rtrim.s` | MIPS assembly: read a string and strip trailing whitespace |
| `erikS.cpp` | OpenGL/X11 platformer source: background/pause rendering, a `Platform` class, tile-based level loader |
| `Global.h` | Header for an OpenGL/X11 side-scroller: a Global class (score, textures, bullet array) plus vector-math macros |
| `build_dirs.sh` | Shell script that scaffolds a class submission tree (`~/3680/wk1..wk15`, projects) |
| `Midterm1.css` / `Midterm1.php` | A web quiz/midterm page and its styling |
