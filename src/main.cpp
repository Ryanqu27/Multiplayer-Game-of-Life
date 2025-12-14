#include "GameWindow.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    int boardRows = 30;
    int boardCols = 40;
    GameWindow gameWindow(boardRows, boardCols);
    gameWindow.runProgram();
    return 0;
}