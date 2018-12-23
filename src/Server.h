#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring> // memset()
#include <string>
#include <functional>
#include <optional>

// Network-related
#include <unistd.h> // close socket
#include <arpa/inet.h> // inet_aton()
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

// System-related
#include <signal.h>
#include <sys/wait.h>

#include "Log.h"


class Server {
    private:
        const std::string m_Name;
        static const std::string m_DefaultName;

        const std::string m_Address;
        const unsigned int m_Port;

        int m_SocketHandle;

        bool m_InitSuccessful;
        bool m_HasStarted;

        /* const std::function<void()> m_Callback; */

        bool init();
        void cleanup();
        // Blocks until there are available packets to receive
        std::string receive(int socketHandle) const;
        bool hasData(int socketHandle) const noexcept;

    public:
        Server(std::string name, std::string address, unsigned int port, bool startImmediately = true);
        Server(std::string address, unsigned int port, bool startImmediately = true);
        virtual ~Server();

        void start() noexcept;
        bool hasMessages() const noexcept;
        std::optional<std::string> popMessage() const noexcept;
};


#endif
