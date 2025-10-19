#include "Board.hpp"
#include <array>
#include <vector>
#include <iostream>
#include <random>

Board::Board(int rows, int col) {
    board.resize(rows, std::vector<Cell>(col));
    this->rows = rows;
    this->col = col;
}
size_t Board::getRows() {
    return rows;
}

size_t Board::getCol() {
    return col;
}


bool Board::isOnBoard(const size_t current_row, const size_t current_col, const size_t board_rows, const size_t board_cols) {
    if (current_row >= board_rows || current_col >= board_cols) {
        return false;
    } else {
        return true;
    }
}

Cell Board::getCell(const int row, const int col) {
    return board[row][col];
}

int Board::getNumBlueNeighbors(const size_t cellRow, const size_t cellCol) {
    if (!isOnBoard(cellRow, cellCol, board.size(), board[0].size())) {
        return 0;
    }

    int blueNeighborCount = 0;
    const int numRows = board.size();
    const int numCols = board[0].size();

    for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
        for (int colOffset = -1; colOffset <= 1; ++colOffset) {
            // Skip the cell itself
            if (rowOffset == 0 && colOffset == 0) continue;

            int neighborRow = static_cast<int>(cellRow) + rowOffset;
            int neighborCol = static_cast<int>(cellCol) + colOffset;

            if (isOnBoard(neighborRow, neighborCol, numRows, numCols) &&
                board[neighborRow][neighborCol].getState() == CellState::Alive &&
                board[neighborRow][neighborCol].getOwner() == Owner::Blue) {
                blueNeighborCount++;
            }
        }
    }

    return blueNeighborCount;
}

int Board::getNumRedNeighbors(const size_t cellRow, const size_t cellCol) {
    if (!isOnBoard(cellRow, cellCol, board.size(), board[0].size())) {
        return 0;
    }

    int redNeighborCount = 0;
    const int numRows = board.size();
    const int numCols = board[0].size();

    for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
        for (int colOffset = -1; colOffset <= 1; ++colOffset) {
            // Skip the cell itself
            if (rowOffset == 0 && colOffset == 0) continue;

            int neighborRow = static_cast<int>(cellRow) + rowOffset;
            int neighborCol = static_cast<int>(cellCol) + colOffset;

            if (isOnBoard(neighborRow, neighborCol, numRows, numCols) &&
                board[neighborRow][neighborCol].getState() == CellState::Alive &&
                board[neighborRow][neighborCol].getOwner() == Owner::Red) {
                redNeighborCount++;
            }
        }
    }

    return redNeighborCount;
}

//If both red neighbors and blue neighbors are either 2 or 3, a 50/50 chance at what ownership the cell becomes
void Board::stepCell(Cell &current, const size_t numRedNeighbors, const size_t numBlueNeighbors) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1); // Range [0, 1]
    
    if (current.getState() == CellState::Alive) {
        if (numRedNeighbors < 2 && numBlueNeighbors < 2) {
            current.setState(CellState::Dead);
            current.setOwner(Owner::None);
            return;
        } 
        else if ((numRedNeighbors == 2 || numRedNeighbors == 3) && (numBlueNeighbors >= 4 || numBlueNeighbors < 2)) {
            current.setState(CellState::Alive);
            current.setOwner(Owner::Red); 
            return;
        }
        else if ((numBlueNeighbors == 2 || numBlueNeighbors == 3) && (numRedNeighbors >= 4 || numRedNeighbors < 2)) {
            current.setState(CellState::Alive);
            current.setOwner(Owner::Blue); 
            return;
        }
        else if (numBlueNeighbors > 4 && numRedNeighbors > 4) {
            current.setState(CellState::Dead);
            current.setOwner(Owner::None);
            return;
        }
        else if ((numBlueNeighbors == 2 || numBlueNeighbors == 3) && (numRedNeighbors == 3 || numRedNeighbors == 2)) {
            int randomNum = distrib(gen);
            if (randomNum == 0) {
                current.setState(CellState::Alive);
                current.setOwner(Owner::Blue);
                return;
            }
            else {
                current.setState(CellState::Alive);
                current.setOwner(Owner::Red); 
                return;
            }
        }  
        else {
            current.setState(CellState::Dead);
            current.setOwner(Owner::None);
        }
    }
    else {
        if (numRedNeighbors == 3 && numBlueNeighbors == 3) {
            int randomNum = distrib(gen);
            if (randomNum == 0) {
                current.setState(CellState::Alive);
                current.setOwner(Owner::Red);
            }
            else {
                current.setState(CellState::Alive);
                current.setOwner(Owner::Blue);
            }
        }
        else if (numRedNeighbors == 3) {
            current.setState(CellState::Alive);
            current.setOwner(Owner::Red);
        }
        else if (numBlueNeighbors == 3) {
            current.setState(CellState::Alive);
            current.setOwner(Owner::Blue);
        }
        else {
            current.setState(CellState::Dead);
            current.setOwner(Owner::None);
        }
    }
}

void Board::stepBoard() {
    std::vector<std::vector<Cell>> nextBoard = board; // copy current state

    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[0].size(); ++j) {
            Cell nextCell = board[i][j];
            stepCell(nextCell, getNumRedNeighbors(i, j), getNumBlueNeighbors(i, j));
            nextBoard[i][j] = nextCell;
        }
    }

    board = std::move(nextBoard); // apply next generation
}

void Board::clear() {
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[0].size(); j++) {
            board[i][j].setState(CellState::Dead);
            board[i][j].setOwner(Owner::None);
        }
    }
}

void Board::toggleRedCell(const int row, const int column) {
    if (isOnBoard(row, column, board.size(), board[0].size()) == false) {
        std::cout << "Out of bounds.";
    } 
    else if (board[row][column].getState() == CellState::Alive) {
        board[row][column].setState(CellState::Dead);
        board[row][column].setOwner(Owner::None);
    } else {
        board[row][column].setState(CellState::Alive);
        board[row][column].setOwner(Owner::Red);
    }
}
void Board::toggleBlueCell(int row, int column) {
    if (isOnBoard(row, column, board.size(), board[0].size()) == false) {
        std::cout << "Out of bounds.";
    } 
    else if (board[row][column].getState() == CellState::Alive) {
        board[row][column].setState(CellState::Dead);
        board[row][column].setOwner(Owner::None);
    } 
    else {
        board[row][column].setState(CellState::Alive);
        board[row][column].setOwner(Owner::Blue);
    }
}

std::size_t Board::getBlueCells() {
    std::size_t count = 0;
    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board[0].size(); j++) {
            if(board[i][j].getOwner() == Owner::Blue) {
                count++;
            }
        }
    }
    return count;
}

std::size_t Board::getRedCells() {
    std::size_t count = 0;
    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board[0].size(); j++) {
            if(board[i][j].getOwner() == Owner::Red) {
                count++;
            }
        }
    }
    return count;
}