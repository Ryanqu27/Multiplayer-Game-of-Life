#include <iostream>
#include "Board.hpp"

enum class Difficulty {
    easy = 1,
    medium = 2,
    hard = 3,
};

class AIPlayer {
private:
    Owner color;
    Difficulty difficulty;
public:
    AIPlayer(Owner color, Difficulty difficulty);
    
    std::vector<std::pair<int,int>> chooseMove(Board& board);
    std::vector<std::pair<int,int>> greedyMove(Board& board, Owner color);
    std::vector<std::vector<std::pair<int,int>>>generateMoveBundles(Board& board);
    std::vector<std::pair<int,int>> miniMax(Board& board);
    std::vector<std::pair<int,int>> monteTreeSearch(Board& board);

    int evaluatePosition(Board& board);
    std::vector<std::pair<int, int>> getLegalMoves(Board& board);
    int countEdgeCells(Board& board);
};