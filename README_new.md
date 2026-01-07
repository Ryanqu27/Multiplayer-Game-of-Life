# Multiplayer Game of Life

An interactive implementation of Conway's Game of Life featuring multiplayer gameplay. Two players compete by toggling cells as either red or blue. Program also features an AI player of three different difficulty levels you can play against. The game ends when either one player is eliminated from the board or 200 generations are complete. The color with the most cells after 200 generations will win.  


## Requirements

- **C++20** or later
- **SDL2**: Cross-platform graphics library
- **OpenGL 3.0+**: GPU rendering

## Building

### Prerequisites

Ensure you have the required tools installed:
- CMake 3.15+
- C++20 compatible compiler
- vcpkg (for dependency management)

### Build Instructions

```bash
# Create and navigate to build directory
cmake --preset default
cmake --build build --config Debug
```

### Running

```bash
./build/bin/game-of-life
```

## How to Play

1. Launch the application
2. Choose your game mode (Player vs. Player or Player vs. AI)
3. Take turns toggling cells on the board
4. Observe as Conway's Game of Life rules apply to the grid
5. Try to have your color dominate

