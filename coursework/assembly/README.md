# Assembly Language

Course labs in MIPS (SPIM) and x86-64 (NASM / GNU as) assembly, moving from basic syscalls to procedures, inline asm, and a final MIPS project. **Languages:** MIPS assembly, x86-64 assembly, C, C++.

| Folder | What it is |
|--------|------------|
| `lab01-mips-hello` | MIPS/SPIM intro: `esotohello.s` prints a string via syscalls; C reference `bits.c` shows int/float bit layout, two's complement, and IEEE-754. |
| `lab02-mips-sum` | Same lab set as `lab01-mips-hello` but `esotosum.s` adds 4+5 and prints the sum with MIPS print-int/print-string syscalls. |
| `lab03-mips-io` | MIPS I/O lab: `main.s` drives a hand-written `printf.s` and `read.s` to prompt for and echo a name, ID, string, and integer. |
| `lab04-mips-procedures` | MIPS procedures/stack-frame lab: recursive Fibonacci (`fact.s`, hardcoded n=7), Euclid GCD from argv (`gcd.s`), and `cmdline.s` multiplying two cmdline ints via atoi/itoa. |
| `lab05-mips-fib-cmdline` | Same lab as `lab04-mips-procedures` but `fact.s` reads n from the command line and prints the recursive Fibonacci result through a custom `printf`. |
| `lab06-mips-calc-gcd` | MIPS lab 6: `calc.s` reads and sums two integers, `gcd.s`, and `sample.s` prints a cmdline char as string, char, and ASCII int. |
| `lab08-x64-scanf-sum` | x86-64 NASM lab: `lab8.asm` reads two ints via glibc scanf, compares them, and prints their sum; Makefile builds bare/hello/goodbye/io/driver examples. |
| `lab09-x64-inline-asm` | x86-64 inline-assembly optimization lab: `lab10.c` uses inline `sal`/`sar` for exponent/log vs brute-force loops, plus a NASM syscall-only hello-world and `gcc -S` reference asm. |
| `project-pascals-triangle` | MIPS final project (CMPS 2240): staged versions `50`-`90` (by grade) building up to computing the binomial coefficient C(n,k) via factorial loops and printing a Pascal's-triangle row from two cmdline ints. |
| `infinate_powers.cpp` | Loose C++ file: prompts for a number (>=1) and prints its square. |
