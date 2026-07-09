# Random C++ and Misc Coursework Files

A grab-bag of one-off C++ exercises, LeetCode solutions, assembly labs, Lisp/Prolog assignments, and stray game-engine source, not a single course. **Languages:** C++, x86-64 Assembly (NASM), MIPS Assembly, Common Lisp, Prolog, Shell, CSS.

| Folder | What it is |
|--------|------------|
| `Global.h` | Header for an OpenGL/X11 "Rambo" side-scroller: Global class (score, textures, bullet array) plus vector-math macros |
| `Makefile` | Assembles/links `Soto_Erik_Lab10.asm` via `nasm -f elf64` + gcc (CMPS 2240) |
| `Midterm1.css` | CSS for a web quiz/midterm page (question, score, legend styling) |
| `Soto_Erik_Lab09.s` | x86-64 NASM: reads two ints, prints the larger and their sum using printf/scanf |
| `Soto_Erik_Lab10.asm` | x86-64 NASM: loops reading numbers and accumulating their sum |
| `best_lab06.cpp` | Single-layer perceptron trained on `iris-binary-normalized.csv` over 200 epochs |
| `build_dirs.sh` | Shell script that scaffolds a class submission tree (`~/3680/wk1..wk15`, projects) and hooks `.bash_logout` |
| `cite.cpp` | MLA citation generator: `CiteGen` class prompts for author/title/publisher/year |
| `erikS.cpp` | OpenGL/X11 platformer source: background/pause-screen rendering, `Platform` class, tile-based level loader and renderer |
| `feed.cpp` | Perceptron feed-forward: weighted-sum vs theta step function, one weight update |
| `lab02.cl` | Common Lisp: iterative/recursive dot printing, count a's, fibonacci, greater, summation (CMPS 3500) |
| `lab03.cl` | Common Lisp: list remove, recursive remove, flatten, palindrome check, prefix-to-infix conversion |
| `lab05.pl` | Prolog: animal-taxonomy facts/rules (likes), max-of-3, Towers-of-Hanoi move count |
| `lab1.pl` | Prolog: forward-chaining item-derivation rules |
| `lab12.cpp` | C++: equal-temperament musical note frequency calculator from A4=440 (CMPS 3350) |
| `lab6.cpp` | C++ debugging exercise: count dictionary words whose 4th letter is 'a' (intentionally buggy) |
| `lcm.cpp` | Least common multiple by brute-force incrementing |
| `median-of-two-sorted-arrays.cpp` | LeetCode: median of two sorted arrays via full merge |
| `membershipfees.cpp` | Yearly-fee/interest loop exercise (intentionally buggy) |
| `miles_per_gallon.cpp` | Prompts gallons and miles, prints MPG |
| `nametester.cpp` | Joke program: prints an insult based on the name entered |
| `prob.cpp` | Integer-overflow demo: counts up from `0xFFFFFFF` forever with printf |
| `randnum.cpp` | Prints a single random number 0-99 |
| `rtrim.s` | MIPS assembly: read a string and strip trailing whitespace |
