#include "Board.hpp"
#include <array>
#include <vector>
#include <iostream>


Board::Board(int rows, int col) {
    board.resize(rows, std::vector<Cell>(col));
    this->rows = rows;
    this->col = col;
}
int Board::getRows() {
    return rows;
}
int Board::getCol() {
    return col;
}
void Board::setCell(const size_t row, const size_t col, Cell cell) {
    board[row][col] = cell;
}
bool Board::isOnBoard(const int current_row, const int current_col, const size_t board_rows, const size_t board_cols) {
    if (current_row >= board_rows || current_col >= board_cols || current_row < 0 || current_col < 0) {
        return false;
    } else {
        return true;
    }
}
Cell Board::getCell(const int row, const int col) {
    return board[row][col];
}

int Board::getNumAliveNeighbors(const size_t row, const size_t col) {
    if (isOnBoard(row, col, board.size(), board[0].size()) == false) {
        return 0;
    }
    int count = 0;
    for (int i = -1; i < 2; i++) {
        if (isOnBoard(row - 1, col + i, board.size(), board[0].size()) == true &&
            board[row - 1][col + i].getState() == CellState::Alive) {
            count++;
        }
    }
    for (int i = -1; i < 2; i += 2) {
        if (isOnBoard(row, col + i, board.size(), board[0].size()) == true &&
            board[row][col + i].getState() == CellState::Alive) {
            count++;
        }
    }
    for (int i = -1; i < 2; i++) {
        if (isOnBoard(row + 1, col + i, board.size(), board[0].size()) == true &&
            board[row + 1][col + i].getState() == CellState::Alive) {
            count++;
        }
    }
    return count;
}

CellState Board::stepCell(const CellState current, const size_t num_neighbors) {
    if (current == CellState::Alive) {
        if (num_neighbors < 2) {
            return CellState::Dead;
        } else if (num_neighbors < 4) {
            return CellState::Alive;
        } else {
            return CellState::Dead;
        }
    } 
    else if (num_neighbors == 3) {
        return CellState::Alive;
    }
    else {
        return CellState::Dead;
    }
}

void Board::stepBoard() {
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[0].size(); j++) {
            board[i][j].setState(stepCell(board[i][j].getState(), getNumAliveNeighbors(i, j)));
        }
    }
}

void Board::clear() {
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[0].size(); j++) {
            board[i][j].setState(CellState::Dead);
        }
    }
}

void Board::toggleCell(const int row, const int column) {
    if (isOnBoard(row, column, board.size(), board[0].size()) == false) {
        std::cout << "Out of bounds.";
    } else if (board[row][column].getState() == CellState::Alive) {
        board[row][column].setState(CellState::Dead);
    } else {
        board[row][column].setState(CellState::Alive);
    }
}
