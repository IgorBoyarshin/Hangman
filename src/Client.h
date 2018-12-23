#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <exception>
#include <cstring> // memset()

// Network-related
#include <unistd.h> // close() socket
#include <arpa/inet.h> // inet_aton()
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include "Log.h"


// A client for Client-Server commutication.
// Communicates via TCP protocol
class Client {
    private:
        const std::string m_Name;
        static const std::string m_DefaultName;

        const std::string m_ServerAddress;
        const unsigned int m_ServerPort;

        // bool init();
        // bool cleanup();

        bool connect(int socketHandle, sockaddr_in& serverSocketInfo) noexcept;

    public:
        Client(std::string name, std::string serverAddress, unsigned int serverPort);
        Client(std::string serverAddress, unsigned int serverPort);
        virtual ~Client();

        bool send(std::string message);
};


#endif
