# Operating Systems Labs (CMPS 3600)

Course labs on processes, signals, and POSIX threads synchronization, mostly in C. **Languages:** C, C++.

| Folder | What it is |
|--------|------------|
| `lab01-fork-signals` | `fork()` a child, use `alarm`/`pause` for signal-driven termination, and count syscalls via `strace` |
| `lab02-signal-handlers` | Signal handlers: SIGALRM ticker plus a version catching SIGINT/SIGFPE (divide-by-zero) that tallies signals and run time |
| `lab05-semaphore-threads` | 7 pthreads gated by a binary semaphore, printing semaphore theory lines in sorted order |
| `lab06-race-conditions` | Race-condition demo: unsynchronized counter loses updates vs. semaphore- and mutex-protected fixes (with report) |
| `lab08-racers-bridge` | The Racers Problem: two threads race across a shared bridge via mutex `trylock`, first to 20000 laps wins |
