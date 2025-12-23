
# ArcadiaEngine

ArcadiaEngine is a C++14 algorithms-and-data-structures “mini engine” built around a game-inspired theme. It bundles multiple core CS topics into one consistent API (hashing, skip lists, red-black trees, dynamic programming, graph algorithms, and greedy scheduling) and provides ready-made test entry points to validate correctness.

This repository is organized like a small library (`ArcadiaEngine.h/.cpp`) plus standalone test executables (`main_test_*.cpp`).

## What’s inside

### Part A — Data Structures

- **PlayerTable**: hash table with **double hashing** (insert + search).
- **Leaderboard**: **skip list** that stores `(playerID, score)` ordered by score (ties broken by smaller `playerID`).
- **AuctionTree**: **red-black tree** for auction items, ordered by `(price, itemID)`.

### Part B — Dynamic Programming (InventorySystem)

- `optimizeLootSplit`: partitions coins into two stacks with minimum difference.
- `maximizeCarryValue`: classic **0/1 knapsack** (max value under capacity).
- `countStringPossibilities`: counts valid decodings for strings containing `u/n` patterns (mod $10^9+7$), returning `0` for invalid characters.

### Part C — Graphs (WorldNavigator)

- `pathExists`: BFS reachability on an undirected graph.
- `minBribeCost`: minimum spanning tree total cost (Prim’s algorithm), returns `-1` if the graph is disconnected.
- `sumMinDistancesBinary`: all-pairs shortest paths (Floyd–Warshall) and returns the sum of pairwise shortest distances as a **binary string**.

### Part D — Greedy (ServerKernel)

- `minIntervals`: task scheduler with cooldown `n`, returns minimum total intervals (including idle time).

## Project layout

- `ArcadiaEngine.h`: public API (interfaces + static algorithm utilities).
- `ArcadiaEngine.cpp`: concrete implementations + factory functions used by tests.
- `main_test_v011.cpp`: default “demo/live” test runner (currently built by CMake).
- `main_test_student.cpp`: extended “happy path” + edge-case test suite.

## Build & run (Windows)

### Option A — CMake (recommended)

This project is configured to compile as a single executable.

1. Configure & build:

	```bash
	cmake -S . -B build
	cmake --build build
	```

2. Run:

	```bash
	.\build\ArcadiaEngine.exe
	```

### Option B — Compile with g++ directly

If you use MSYS2/MinGW (as your VS Code task suggests):

```bash
g++ -std=c++14 -O2 -Wall -Wextra ArcadiaEngine.cpp main_test_v011.cpp -o ArcadiaEngine.exe
./ArcadiaEngine.exe
```

## Switching test runners

By default, `CMakeLists.txt` builds `main_test_v011.cpp`. To switch to the student test runner:

1. Open `CMakeLists.txt`
2. Replace `main_test_v011.cpp` with `main_test_student.cpp`
3. Rebuild:

```bash
cmake --build build
```

## Notes

- Language standard: **C++14** (see `CMakeLists.txt`).
- The tests rely on factory functions declared with `extern "C"`:
  - `createPlayerTable()`
  - `createLeaderboard()`
  - `createAuctionTree()`

