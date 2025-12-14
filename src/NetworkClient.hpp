#pragma once
#include <string>
#include <thread>
#include <functional>
#include <atomic>
#include <memory>
#include <cstdint>
#include <asio.hpp>
#include <json/json.h>
#include "GameSession.hpp"
#include <queue>

enum class MessageType {
    JoinGame,
    CellToggle,
    BoardSync,
    GameStart,
    GameEnd,
    Disconnect
};

struct NetworkMessage {
    MessageType type;
    std::string senderID;
    std::string gameSessionID;

    int row;
    int col;
    Owner owner;
    long long timestamp;  
};

class NetworkClient {
private:
    std::string playerID;
    std::string serverAddress;
    uint16_t serverPort;
    int socketFD;

    std::queue<NetworkMessage> incomingMessages;
    std::mutex messageMutex;

    std::thread receiverThread;
    bool isConnected;
    bool stopReceiverThread;

public:
    explicit NetworkClient(const std::string& playerID);
    ~NetworkClient();
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    bool connectToServer(const std::string& address, uint16_t port);
    void disconnect();

    bool isConnectedToServer() const;
    void sendCellToggle(int row, int col, Owner owner, const std::string& gameSessionID);
    void sendGameStart(const std::string& gameSessionID);
    void sendGameEnd(const std::string& gameSessionID);

    void sendMessage(const NetworkMessage& message);
    bool getNextMessage(NetworkMessage& message);
    bool hasMessages() const;

private:
    void receiverThreadLoop();
    void processIncomingData(const std::string& rawData);
    std::string serializeMessage(const NetworkMessage& message) const;
    NetworkMessage deserializeMessage(const std::string& data) const;
};

