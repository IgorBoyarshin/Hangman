#include "NetworkManager.h"


NetworkManager::NetworkManager(
        const std::string& name,
        const std::string& address,
        const unsigned int port)
    : m_Server(new Server(name, address, port, true)),
      m_ServerAddress(address), m_ServerPort(port) {
    Log::info().setClass("NetworkManager").setFunc("NetworkManager")
        << "NetworkManager " << name << " has started on address "
        << address << ":" << port << std::endl;
}

NetworkManager::~NetworkManager() {
    delete m_Server;
}

std::optional<std::string> NetworkManager::receive() {
    return m_Server->popMessage();
}

std::string NetworkManager::blockReceive() {
    while (!m_Server->hasMessages()) {
        static const unsigned int SLEEP_MILLIS = 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLIS));
    }
    return *m_Server->popMessage();
}

bool NetworkManager::send(const std::string& addr, unsigned int port, const std::string& message) {
    Client client("DefaultClient", addr, port);
    return client.send(message);
}

// TODO
bool NetworkManager::establishConnection(const std::string& ip, const std::string& port) {
    return false;
}

// TODO
bool NetworkManager::connectionEstablished() {
    return false;
}

std::string NetworkManager::getAddress() const {
    return m_ServerAddress;
}

unsigned int NetworkManager::getPort() const {
    return m_ServerPort;
}
