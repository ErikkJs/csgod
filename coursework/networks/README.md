# Computer Networks Labs

TCP/IP socket programming in C on Linux, building from a blocking client/server up to `select()`-based multiplexing and signal handling. **Languages:** C.

| Folder | What it is |
|--------|------------|
| `lab2` | Blocking TCP chat pair: `vcrec` server (socket/bind/getsockname/listen/accept) and `vcsend` client (connect), exchanging null-terminated lines with a handshake and blocking `recv()`. |
| `lab3` | Standalone `select.c` demo: blocks SIGCHLD/SIGQUIT, catches SIGINT (^C) with a handler, and does event-driven stdin reads via `select()` with a 30s timeout. |
| `lab4` | The `vcsend`/`vcrec` pair rewritten to use `select()` (plus `select_loop.c`) to multiplex stdin and the socket for non-blocking full-duplex messaging. |
