#include "Cell.hpp"

Cell::Cell(CellState state, Owner owner) noexcept {
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


Owner Cell::getOwner() const noexcept { 
    return owner; 
}

void Cell::setOwner(Owner newOwner) noexcept { 
    this->owner = newOwner; 
}