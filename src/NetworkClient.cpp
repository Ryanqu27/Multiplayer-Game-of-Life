#include "NetworkClient.hpp"
#include <iostream>
#include <sstream>
#include <json/json.h>
/*
NetworkClient::NetworkClient(const std::string& playerID)
    : playerID(playerID),
    serverAddress(""),
    serverPort(0),
    socketFD(-1),
    isConnected(false),
    stopReceiverThread(false)
    {}
NetworkClient::~NetworkClient() {
    disconnect();
}

bool NetworkClient::connectToServer(const std::string& address, uint16_t port) {
    serverAddress = address;
    serverPort = port;

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFD == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return false;
    }
    
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

    result = connect(socketFD, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to connect" << std::endl;
        closesocket(socketFD);
        WSACleanup();
        return false;
    }
    isConnected = true;
    
    stopReceiverThread = false;
    receiverThread = std::thread(&NetworkClient::receiverThreadLoop, this);
    return true;
}

void NetworkClient::disconnect() {
    if (isConnected) {
        isConnected = false;
        stopReceiverThread = true;
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
        if (socketFD != -1) {
            closesocket(socketFD);
            socketFD = -1;
        }

        WSACleanup();
    }
}

bool NetworkClient::isConnectedToServer() const {
    return isConnected;
}

void NetworkClient::sendCellToggle(int row, int col, Owner owner, const std::string& gameSessionID) {
    NetworkMessage message;
    message.type = MessageType::CellToggle;
    message.senderID = playerID;
    message.gameSessionID = gameSessionID;
    message.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    message.row = row;
    message.col = col;
    message.owner = owner;
    
    sendMessage(message);
}

void NetworkClient::sendGameStart(const std::string& gameSessionID) {
    NetworkMessage message;
    message.type = MessageType::GameStart;
    message.gameSessionID = gameSessionID;
    message.senderID = playerID;
    message.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    sendMessage(message);
}

void NetworkClient::sendGameEnd(const std::string& gameSessionID) {
    NetworkMessage message;
    message.type = MessageType::GameEnd;
    message.gameSessionID = gameSessionID;
    message.senderID = playerID;
    message.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    sendMessage(message);
}

void NetworkClient::sendMessage(const NetworkMessage& message) {
    if (!isConnected) {
        std::cerr << "Cannot send messsage. Not connected" << std::endl;
        return;
    }
    Json::Value root;
    root["type"] = static_cast<int>(message.type);
    root["senderID"] = message.senderID;
    root["gameSessionID"] = message.gameSessionID;
    root["timestamp"] = static_cast<Json::Int64>(message.timestamp);

    if (message.type == MessageType::CellToggle) {
        root["row"] = message.row;
        root["col"] = message.col;
        root["owner"] = static_cast<int>(message.owner);
    }
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["emitUTF8"] = true;
    std::string payload = Json::writeString(writerBuilder, root);

    uint32_t len = static_cast<uint32_t>(payload.size());
    uint32_t netLen = htonl(len);
    const char* lenPtr = reinterpret_cast<const char*>(&netLen);

}

*/