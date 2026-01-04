#pragma once
#include <vector>
#include "Cell.hpp"

enum class GameResult: int {
    None = 0,
    Red = 1, 
    Blue = 2,
    Tie = 3
};

class Board {
private:
    std::vector<std::vector<Cell>> board;
    int rows;
    int cols;
    Owner playerTurn;
    std::size_t placedRedCells;
    std::size_t placedBlueCells;
    GameResult gameResult;
    std::size_t numGenerations;
public:
    Board(int rows, int col);
    int getRows();
    int getCol();

    bool isOnBoard(const size_t currentRow, const size_t currentCol, const size_t boardRows, const size_t boardCols);
    Cell getCell(const int row, const int col);
    void stepCell(Cell &cell, const size_t redNeighbors, const size_t blueNeighbors);

    int getNumBlueNeighbors(size_t row, size_t col);
    int getNumRedNeighbors(size_t row, size_t col);

    void stepBoard();
    void reset();

    void toggleRedCell(int row, int column);
    void toggleBlueCell(int row, int column);

    std::size_t getBlueCells() const;
    std::size_t getRedCells() const;
    std::size_t getNumGenerations() const;

    Owner getPlayerTurn();
    void placeRedCell();
    void placeBlueCell();

    GameResult checkGameResult();
    GameResult getGameResult() const;

    int countEmptyNeighborsWithBlueAdjacency(int row, int col);
    int countEmptyNeighborsWithRedAdjacency(int row, int col);
    int countBlueClusters();
};