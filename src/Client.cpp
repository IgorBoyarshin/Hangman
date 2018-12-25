#include "Client.h"


const std::string Client::m_DefaultName = "DefaultClient";


Client::Client(std::string name, std::string serverAddress, unsigned int serverPort)
        : m_Name(name), m_ServerAddress(serverAddress), m_ServerPort(serverPort) {}


Client::Client(std::string serverAddress, unsigned int serverPort)
        : Client(m_DefaultName, serverAddress, serverPort) {}


Client::~Client() {
    Log::info().setClass("Client").setFunc("~Client()")
        << "Client " << m_Name << " destructing" << std::endl;
}


bool Client::connect(int socketHandle, sockaddr_in& serverSocketInfo) noexcept {
    static const unsigned int RETRIES_COUNT = 5;
    for (unsigned int i = 0; i < RETRIES_COUNT; i++) {
        if (::connect(socketHandle,
                    reinterpret_cast<sockaddr*>(&serverSocketInfo),
                    sizeof(serverSocketInfo)) < 0) {
            Log::error().setClass("Client").setFunc("connect()")
                << "Client " << m_Name << ": Could not establish connection. Retrying..." << std::endl;
            static const unsigned int SECONDS = 1;
            sleep(SECONDS);
        } else {
            return true;
        }
    }
    Log::error().setClass("Client").setFunc("connect()")
        << "Client " << m_Name << ": Could not establish connection. Retries count exhausted." << std::endl;

    return false;
}


bool Client::send(std::string message) {
    Log::info().setClass("Client").setFunc("send()")
        << "Client " << m_Name << " tries to send:" << message << std::endl;

    // AF_INET === IPV4, AF_INET6 === IPV6
    // SOCK_STREAM === TCP, SOCK_DGRAM === UDP
    // IPPROTO_TCP === IP (Internet Protocol)
    int socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketHandle < 0) {
        Log::error().setClass("Client").setFunc("send()")
            << "Client " << m_Name << ": Could not create socket." << std::endl;
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
    Log::info().setClass("Client").setFunc("send()")
        << "Client " << m_Name << ": Connection to server established" << std::endl;

    // TODO: Handle send error
    ::send(socketHandle, message.c_str(), message.size(), 0);
    Log::info().setClass("Client").setFunc("send()")
        << "Client " << m_Name << " has sent:" << message << std::endl;

    close(socketHandle);

    return true;
}
