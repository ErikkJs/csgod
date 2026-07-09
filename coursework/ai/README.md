# Artificial Intelligence (CMPS 3560)

Labs from an intro AI course mixing C++ agents (reflex classifier, genetic algorithm) with Prolog symbolic AI (expert systems, certainty factors, fuzzy logic); the `other/` folder is a separate, earlier standalone Prolog track covering the same symbolic-AI topics. **Languages:** C++, Prolog.

| Folder | What it is |
|--------|------------|
| `lab1` | C++ simple reflex agent that classifies Iris setosa vs. versicolor from a CSV and reports confusion-matrix metrics (TPR, sensitivity, specificity, precision, accuracy). |
| `lab2` | Prolog "MEDIA Advisor" rule-based expert system: a 20-questions backward-inference chatbot recommending a training medium (lecture/workshop/videocassette/roleplay); includes the classic animal-guessing demo. |
| `lab3` | Prolog certainty-factor inference network that propagates CF values through an item/rule chain via `min()`, plus a facts-and-rules teaching example. |
| `lab4` | Prolog fuzzy logic: height membership functions (short/average/tall) with linguistic hedges (little/slightly/very/extremely); nested `lab4/` adds fuzzy AND/OR/NOT and De Morgan's. |
| `lab8` | C++ genetic algorithm solving OneMax (evolve a 10x10-bit population to all 1s) with tournament selection, single-point crossover, and bit-flip mutation. |
| `lab8.cpp` | Standalone working single-file version of the OneMax genetic algorithm (the finished counterpart to the `lab8/` draft). |
| `other` | Separate earlier Prolog course: recursion/backtracking basics (lab1), the media-advisor expert system (lab2), certainty factors (lab3), fuzzy logic (lab4), plus a C++ evolution-strategy cubic-polynomial fit (lab9). |
