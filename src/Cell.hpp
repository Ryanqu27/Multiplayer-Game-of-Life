#pragma once

#include <cstdint>

enum class CellState : bool { Dead = false, Alive = true };

// A Cell represents a single square on the board.
class Cell {
private:
    CellState state;
    int owner; // 0 = red cell. 1 = blue cell. -1 = dead cell
public:
    // Construct a cell. Default is dead.
    explicit Cell(CellState state = CellState::Dead, int owner = -1) noexcept;

    [[nodiscard]] CellState getState() const noexcept;
    void setState(CellState s) noexcept;

    [[nodiscard]] bool isAlive() const noexcept;
    void toggle() noexcept;

    [[nodiscard]] int getOwner() const noexcept;
    void setOwner(int owner) noexcept;
};
