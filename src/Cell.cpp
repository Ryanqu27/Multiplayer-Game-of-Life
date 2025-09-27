#include "Cell.hpp"

Cell::Cell(CellState state, int owner) noexcept {
    this->owner = owner;
    this->state = state;
}

CellState Cell::getState() const noexcept { 
    return state; 
}

void Cell::setState(CellState s) noexcept { 
    state = s; 
}

bool Cell::isAlive() const noexcept { 
    return state == CellState::Alive; 
}

void Cell::toggle() noexcept { 
    if (state == CellState::Dead) {
        state = CellState::Alive;
    }
    else {
        state = CellState::Dead;
    }
}

int Cell::getOwner() const noexcept { 
    return owner; 
}

void Cell::setOwner(int owner) noexcept { 
    this->owner = owner; 
}