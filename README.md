# Coup Game

A Qt-based graphical implementation of a strategic role-playing game inspired by **Coup**, implemented in modern C++ with a full game engine and GUI.  
Supports all player roles, actions, and interactive gameplay logic.

---

## Features

- **Complete turn-based game engine** (`Game`, `Player`, `Governor`, etc.).
- **Graphical UI** using Qt5 – dynamic buttons, labels, interaction per player turn.
- **Full rule logic**: turns, bribes, taxes, coups, roles, win conditions.
- **Exception-safe**: all invalid actions throw descriptive exceptions.
- **Unit-tested** with Doctest.
- **Memory-safe** (tested with Valgrind).

---

## Roles and Actions

| Role      | Special Abilities |
|-----------|-------------------|
| **Governor** | Tax = 3, can undo another player's `tax` |
| **Spy**      | Can spy (view coins), block `arrest` |
| **Baron**    | `invest`: 3 → 6 coins, gains on `sanction` |
| **General**  | Can block `coup` for 5 coins, recovers coins on `arrest` |
| **Judge**    | Can undo `bribe`, extra penalty on `sanction` |
| **Merchant** | Bonus coin if ≥3 coins at turn start, avoids `arrest` loss |

All core actions are implemented:
- `gather` (1 coin)
- `tax` (2 coins or 3 for Governor)
- `bribe` (pay 4 to act again)
- `arrest` (steal 1 coin from another)
- `sanction` (blocks target's income actions)
- `coup` (eliminate a player for 7 coins)

---

## Requirements

- C++17 or newer  
- Qt5 libraries (`qtbase5-dev`, `libqt5widgets5`)  
- `g++`, `make`  
- `valgrind` for memory checks  
- [`doctest.h`](https://github.com/doctest/doctest) for tests

---

## Build & Run

```bash
# 1. Build and launch the GUI:
make 
./CoupGame

# 2. Run unit tests:
make test

# 3. Memory check (Valgrind):
make valgrind

# 4. Cleanup build files:
make clean

## Project Structure
├── Game.cpp / Game.hpp         # Core engine
├── Player.hpp / roles/*.hpp    # Base and derived roles
├── GameWindow.cpp/.h           # Main Qt window
├── StartDialog.cpp/.h          # Player creation dialog
├── GameQtBridge.hpp            # Bridge logic for Qt
├── test.cpp                    # All unit tests
├── Makefile                    # All build targets
└── README.md                   # This file

