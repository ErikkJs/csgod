# C#

A single C# lab demonstrating polymorphism via an `IShape` interface implemented by Rectangle, Triangle, and Circle classes with area computation. **Languages:** C#, Make.

| Folder | What it is |
|--------|------------|
| `lab14.cs` | Console app: `IShape` interface with Rectangle/Triangle/Circle implementations, each computing `Area` and a `Display()`; Circle exposes a read/write `Radius` property |
| `Makefile` | Builds `lab14` with the Mono C# compiler (`mcs`) plus a `hw14` C target and a `clean` rule |
| `out` | Captured console output from running `lab14` (shape areas and per-shape descriptions) |
