#include "Client.h"


const std::string Client::m_DefaultName = "DefaultClient";


Client::Client(std::string name, std::string serverAddress, unsigned int serverPort)
        : m_Name(name), m_ServerAddress(serverAddress), m_ServerPort(serverPort) {}


Client::Client(std::string serverAddress, unsigned int serverPort)
        : Client(m_DefaultName, serverAddress, serverPort) {}


Client::~Client() {
    std::cout << "[LOG:INFO:~Client()]: " << m_Name << " destructing." << std::endl;
}


bool Client::connect(int socketHandle, sockaddr_in& serverSocketInfo) noexcept {
    static const unsigned int RETRIES_COUNT = 5;
    for (unsigned int i = 0; i < RETRIES_COUNT; i++) {
        if (::connect(socketHandle,
                    reinterpret_cast<sockaddr*>(&serverSocketInfo),
                    sizeof(serverSocketInfo)) < 0) {
            std::cout << "[LOG:ERR:Client:connect]" << "Could not establish connection. Retrying..." << std::endl;
            const unsigned int SECONDS = 1;
            sleep(SECONDS);
        } else {
            return true;
        }
    }
    std::cout << "[LOG:ERR:Client:connect]" << "Could not establish connection. Retries count exhausted." << std::endl;

    return false;
}


bool Client::send(std::string message) {
    std::cout << "[LOG:INFO]" << "send() in client " << m_Name << std::endl;

    // AF_INET === IPV4, AF_INET6 === IPV6
    // SOCK_STREAM === TCP, SOCK_DGRAM === UDP
    // IPPROTO_TCP === IP (Internet Protocol)
    int socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketHandle < 0) {
        std::cout << "[LOG:ERR:Client]" << "Could not create socket." << std::endl;
        return false;
    }

    // Initialize struct containing destination (server) info
    sockaddr_in serverSocketInfo;
    memset(&serverSocketInfo, 0, sizeof(sockaddr_in));
    serverSocketInfo.sin_family = AF_INET;
    serverSocketInfo.sin_port = htons(m_ServerPort);
    inet_aton(m_ServerAddress.c_str(), &serverSocketInfo.sin_addr);

    // Try to connect
    if (!connect(socketHandle, serverSocketInfo)) return false;
    std::cout << "[LOG:INFO]" << "Connection to server established" << std::endl;

    // TODO: Handle send error
    ::send(socketHandle, message.c_str(), message.size(), 0);
    std::cout << "[LOG:INFO:Client:send]" << "sent: " << message << std::endl;

    close(socketHandle);

    return true;
}
