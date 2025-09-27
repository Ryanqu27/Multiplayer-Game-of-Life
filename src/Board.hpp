#pragma once
#include <vector>
#include "Cell.hpp"

class Board {
    std::vector<std::vector<Cell>> board;
    int rows;
    int col;

    public:
        Board(int rows, int col);
        int getRows();
        int getCol();
        void setCell(const size_t row, const size_t col, Cell cell);
        bool isOnBoard(int current_row, int current_col, size_t board_rows, size_t board_cols);
        Cell getCell(const int row, const int col);
        int getNumAliveNeighbors(size_t row, size_t col);
        CellState stepCell(CellState current, size_t num_neighbors);
        void stepBoard();
        void clear();
        void toggleCell(int row, int column);

};