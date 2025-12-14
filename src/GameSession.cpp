#include "GameSession.hpp"

GameSession::GameSession(GameSessionInfo& info, Board& gameBoard)
    : sessionInfo(info), gameBoard(gameBoard), isActive(false){}

const GameSessionInfo& GameSession::getSessionInfo() const {
    return this->sessionInfo;
}
const Board& GameSession::getBoard() const {
    return this->gameBoard;
}

// Game state management
bool GameSession::isGameActive() const {
    return this->isActive;
}
void GameSession::startGame() {
    isActive = true;
}
void GameSession::endGame() {
    isActive = false;
}

bool GameSession::isLocalPlayer(const std::string& playerID) const {
    return sessionInfo.playerID == playerID;
}