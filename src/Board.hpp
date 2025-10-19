#pragma once
#include <vector>
#include "Cell.hpp"

class Board {
    std::vector<std::vector<Cell>> board;
    int rows;
    int col;

    public:
        Board(int rows, int col);
        std::size_t getRows();
        std::size_t getCol();

        bool isOnBoard(size_t current_row, size_t current_col, size_t board_rows, size_t board_cols);
        Cell getCell(const int row, const int col);
        void stepCell(Cell &cell, const size_t redNeighbors, const size_t blueNeighbors);

        int getNumBlueNeighbors(size_t row, size_t col);
        int getNumRedNeighbors(size_t row, size_t col);

        void stepBoard();
        void clear();

        void toggleRedCell(int row, int column);
        void toggleBlueCell(int row, int column);

        std::size_t getBlueCells();
        std::size_t getRedCells();

};