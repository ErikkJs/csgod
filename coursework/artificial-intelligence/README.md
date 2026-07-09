# Artificial Intelligence (CMPS 3560)

Labs from an intro AI course mixing C++ agents (reflex classifier, genetic algorithm) with Prolog symbolic AI (expert systems, certainty factors, fuzzy logic). The `prolog-symbolic-ai/` folder is a separate, earlier standalone Prolog track covering the same symbolic-AI topics. **Languages:** C++, Prolog.

| Folder | What it is |
|--------|------------|
| `lab01-reflex-classifier` | C++ simple reflex agent that classifies Iris setosa vs. versicolor from a CSV and reports confusion-matrix metrics (TPR, sensitivity, specificity, precision, accuracy). |
| `lab02-expert-system` | Prolog "MEDIA Advisor" rule-based expert system: a 20-questions backward-inference chatbot recommending a training medium (lecture/workshop/videocassette/roleplay); includes the classic animal-guessing demo. |
| `lab03-certainty-factors` | Prolog certainty-factor inference network that propagates CF values through an item/rule chain via `min()`, plus a facts-and-rules teaching example. |
| `lab04-fuzzy-logic` | Prolog fuzzy logic: height membership functions (short/average/tall) with linguistic hedges (little/slightly/very/extremely); nested `lab4/` adds fuzzy AND/OR/NOT and De Morgan's. |
| `lab08-genetic-algorithm` | C++ genetic algorithm solving OneMax (evolve a 10x10-bit population to all 1s) with tournament selection, single-point crossover, and bit-flip mutation. |
| `onemax-final.cpp` | Standalone working single-file version of the OneMax genetic algorithm (the finished counterpart to the `lab08-genetic-algorithm/` draft). |
| `prolog-symbolic-ai/` | Separate earlier Prolog track — `lab01-prolog-basics` (recursion/backtracking), `lab02-media-advisor` (expert system), `lab03-certainty-factors`, `lab04-fuzzy-logic`, and `lab09-evolution-strategy` (a C++ evolution-strategy cubic-polynomial fit). |
