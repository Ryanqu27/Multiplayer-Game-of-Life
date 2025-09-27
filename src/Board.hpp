#pragma once
#include <vector>
#include "Cell.hpp"
using std::size_t;

class Board {
    std::vector<std::vector<Cell>> board;
    int rows;
    int col;

    public:
        Board(int rows, int col);
        size_t getRows();
        size_t getCol();

        void setCell(const size_t row, const size_t col, Cell cell);
        bool isOnBoard(size_t current_row, size_t current_col, size_t board_rows, size_t board_cols);
        Cell getCell(const int row, const int col);
        CellState stepCell(CellState current, size_t num_neighbors);

        int getNumAliveNeighbors(size_t row, size_t col);
        void stepBoard();
        void clear();
        void toggleCell(int row, int column);

};