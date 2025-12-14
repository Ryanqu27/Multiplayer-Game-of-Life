#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Board.hpp"
#include <iostream>

enum class GameRole {
    Server, Client, Spectator
};

struct GameSessionInfo {
    std::string sessionID;
    std::string playerID;
    GameRole role;
    std::string opponentID;
    int boardRows;
    int boardCols;
};

class GameSession {
private:
    GameSessionInfo sessionInfo;
    Board gameBoard;
    bool isActive;
public:
    GameSession(GameSessionInfo& info, Board& gameBoard);
    
    const GameSessionInfo& getSessionInfo() const;
    const Board& getBoard() const;

    // Game state management
    bool isGameActive() const;
    void startGame();
    void endGame();
    
    bool isLocalPlayer(const std::string& playerID) const;
};