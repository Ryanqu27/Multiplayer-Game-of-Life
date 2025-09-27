#include <iostream>
#include <vector>
#include "Board.hpp"
#include "Cell.hpp"

static void printStateBoard(Board& board) {
    for (size_t i = 0; i < board.getRows(); ++i) {
        for (size_t j = 0; j < board.getCol(); ++j) {
            std::cout << (board.getCell(i, j).getState() == CellState::Alive ? 'O' : '.');
        }
        std::cout << '\n';
    }
}

int main() {
    // Test Cell class
    Cell c1(CellState::Alive, 1);
    Cell c2(CellState::Dead, -1);

    c2.setState(CellState::Alive);
    c2.setOwner(2);
    

    Board board(5, 5);
    board.setCell(0, 0, c1);
    board.setCell(0,1,c2);
    board.setCell(1,0,c1);


    std::cout << "Initial state:\n";
    printStateBoard(board);

    board.stepBoard();

    std::cout << "After one step:\n";
    printStateBoard(board);

    return 0;
}