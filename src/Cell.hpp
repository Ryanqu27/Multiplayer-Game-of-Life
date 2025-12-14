#pragma once
#include <cstdint>

enum class CellState : bool { Dead = false, Alive = true };
enum class Owner : int { None = 0, Red = 1, Blue = 2 };

class Cell {
private:
    //If cell is dead, owner is none.
    CellState state;
    Owner owner; 
public:
    // Construct a cell. Default is dead.
    explicit Cell(CellState state = CellState::Dead, Owner owner = Owner::None) noexcept;

    [[nodiscard]] CellState getState() const noexcept;
    void setState(CellState s) noexcept;

    [[nodiscard]] bool isAlive() const noexcept;

    [[nodiscard]] Owner getOwner() const noexcept;
    void setOwner(Owner owner) noexcept;
};
