#include "NetworkManager.h"


NetworkManager::~NetworkManager() {}

// TODO
std::optional<std::string> NetworkManager::receiveMessage() {
    return std::nullopt;
}

// TODO
bool NetworkManager::sendMessage(const std::string& message) {
    return false;
}

// TODO
bool NetworkManager::establishConnection(const std::string& ip, const std::string& port) {
    return false;
}

// TODO
bool NetworkManager::connectionEstablished() {
    return false;
}
