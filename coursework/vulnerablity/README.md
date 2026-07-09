# Software Security / Vulnerability Coursework

Labs on debugging crashes, mapping C++ to x86-64 assembly, and reverse-engineering stripped binaries. **Languages:** C++, x86-64 assembly.

| Folder | What it is |
|--------|------------|
| `hw3` | Templated quicksort (median-of-three pivot, insertion sort below a size threshold) with ANSI-color step-by-step debug output |
| `lab1` | GDB crash-debugging exercise: a program that reads into an unallocated (NULL) array and segfaults, kept alongside its core dump and a hex dump of the ELF |
| `lab3` | Tiny C++ control-structure snippets (if / for / while / void-call / return) compiled and disassembled in GDB to study the generated x86-64 code |
| `lab4` | Stripped x86-64 ELF reverse-engineering target (`lab4_binary_sleipnir`, uses strncat/strlen/memset) for static/dynamic binary analysis |
