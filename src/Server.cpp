#include "Server.h"


const std::string Server::m_DefaultName = "DefaultServer";


Server::Server(std::string name, std::string address,
        unsigned int port, bool startImmediately/* = true*/)
    : m_Name(name), m_Address(address), m_Port(port), m_SocketHandle(0),
      m_InitSuccessful(false), m_HasStarted(false) {
    if (!(m_InitSuccessful = init())) return; // TODO: throw
    if (startImmediately) start();
}


Server::Server(std::string address, unsigned int port, bool startImmediately/* = true*/)
        : Server(m_DefaultName, address, port, startImmediately) {}


Server::~Server() {
    cleanup();
    Log::info().setClass("Server").setFunc("~Server")
        << "Server " << m_Name << " is destructed" << std::endl;
}


void Server::cleanup() {
    close(m_SocketHandle);
}


void Server::start() noexcept {
    if (m_HasStarted) {
        Log::error().setClass("Server").setFunc("start")
            << "Server " << m_Name << " has already been started. Ignoring." << std::endl;
        return;
    }
    // TODO: assert m_SocketHandle is valid

    // Currently, the client is stalled if the queue is overflowed, so try
    // to keep MAX_BUFF_SIZE relatively big
    const unsigned int MAX_IN_QUEUE = 16;
    listen(m_SocketHandle, MAX_IN_QUEUE);
    Log::info().setClass("Server").setFunc("start")
        << "Server " << m_Name << " now listening for connections." << std::endl;
    m_HasStarted = true;
}


bool Server::init() {
    static const auto handleSignal = [](int signum){
        while (waitpid(-1, NULL, WNOHANG) > 0);
    };
    signal(SIGCHLD, handleSignal); // TODO: check if needed

    Log::info().setClass("Server").setFunc("init")
        << "Server " << m_Name << ": initializing." << std::endl;

    // AF_INET === IPV4, AF_INET6 === IPV6
    // SOCK_STREAM === TCP, SOCK_DGRAM === UDP
    // IPPROTO_TCP === IP (Internet Protocol)
    m_SocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_SocketHandle < 0) {
        Log::error().setClass("Server").setFunc("init")
            << "Server " << m_Name << ": Could not create socket." << std::endl;
        m_SocketHandle = 0;
        return false;
    }

    // Settings
    {
        // To release the resource (port) immediately after closing it.
        // Prevents bind error when trying to bind to the same port having closed it.
        int option = 1; // TODO: confirm that the variable may be disposed of
        setsockopt(m_SocketHandle, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    }

    // Initialize struct containing self (server) info
    sockaddr_in selfSocketInfo;
    memset(&selfSocketInfo, 0, sizeof(sockaddr_in));
    selfSocketInfo.sin_family = AF_INET;
    selfSocketInfo.sin_port = htons(m_Port);
    inet_aton(m_Address.c_str(), &selfSocketInfo.sin_addr);

    if (bind(m_SocketHandle, reinterpret_cast<sockaddr*>(&selfSocketInfo),
            sizeof(selfSocketInfo)) < 0) {
        Log::error().setClass("Server").setFunc("init")
            << "Server " << m_Name << ": Unable to bind: " << strerror(errno) << std::endl;
        close(m_SocketHandle);
        m_SocketHandle = 0;
        return false;
    }

    return true;
}


// TODO: make it throw on error
std::string Server::receive(int socketHandle) const {
    if (!m_HasStarted) { // TODO: check m_InitSuccessful too??
        Log::error().setClass("Server").setFunc("receive")
            << "Server " << m_Name << " hasn't been started. Ignoring." << std::endl;
        return "";
    }

    // Initialize struct containing incoming (client) info
    sockaddr_in incomingSocketInfo;
    socklen_t incomingSocketSize = sizeof(incomingSocketInfo);
    int socketConnection = accept(
            socketHandle,
            reinterpret_cast<sockaddr*>(&incomingSocketInfo),
            &incomingSocketSize);
    if (socketConnection < 0) {
        Log::error().setClass("Server").setFunc("receive")
            << "Server " << m_Name << ": unable to accept connections" << std::endl;
        close(socketHandle);
        return "";
    }

    static const unsigned int MAX_BUFF_SIZE = 1024;
    char buff[MAX_BUFF_SIZE];
    int receivedBytesCount = recv(socketConnection, buff, MAX_BUFF_SIZE, 0);
    memset(buff + receivedBytesCount, 0,
        (MAX_BUFF_SIZE - receivedBytesCount) * sizeof(char)); // zero the remaining chunk

    const std::string received = std::string(buff, receivedBytesCount);

    Log::info().setClass("Server").setFunc("receive")
        << "Server " << m_Name << ": Received packet from "
        << inet_ntoa(incomingSocketInfo.sin_addr) << ":"
        << received
        << std::endl;

    close(socketConnection);

    return received;
}


bool Server::hasData(int socketHandle) const noexcept {
    if (!m_HasStarted) { // TODO: check m_InitSuccessful too??
        Log::error().setClass("Server").setFunc("hasData")
            << "Server " << m_Name << " hasn't been started. Ignoring." << std::endl;
        return false;
    }

    fd_set checkReadSet;
    timeval timeout = {0,0}; // results in immediate return (used for polling) if no data ready

    FD_ZERO(&checkReadSet);
    FD_SET(socketHandle, &checkReadSet);

    if (const int readyDescriptorsAmount = select(socketHandle + 1, &checkReadSet,
                static_cast<fd_set*>(0), static_cast<fd_set*>(0), &timeout);
            readyDescriptorsAmount > 0) {
        return FD_ISSET(socketHandle, &checkReadSet);
    } else {
        if (readyDescriptorsAmount == -1) { // error
            Log::error().setClass("Server").setFunc("hasData")
                << "Server " << m_Name << ": select() returned -1:" << strerror(errno) << std::endl;
            close(socketHandle); // do this??
        } // otherwise == 0 => no fitting descriptors found => no data ready
        return false;
    }
}


bool Server::hasMessages() const noexcept {
    return hasData(m_SocketHandle);
}


std::optional<std::string> Server::popMessage() const noexcept {
    if (hasMessages()) {
        return {receive(m_SocketHandle)};
    }
    return std::nullopt;
}
