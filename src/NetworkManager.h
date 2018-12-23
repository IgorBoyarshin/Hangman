#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

// For sleeping
#include <chrono>
#include <thread>

#include <string>

#include "Communicator.h"
#include "Client.h"
#include "Server.h"


// TODO
class NetworkManager : public Communicator {
    private:
        Server* m_Server;
        const std::string m_ServerAddress;
        const unsigned int m_ServerPort;

    public:
        NetworkManager(const std::string& name, const std::string& address, const unsigned int port);
        virtual ~NetworkManager();
        std::optional<std::string> receive() override;
        std::string blockReceive() override;
        bool send(const std::string& addr, unsigned int port, const std::string& message) override;
        bool establishConnection(const std::string& ip, const std::string& port) override;
        bool connectionEstablished() override;

        std::string getAddress() const override;
        unsigned int getPort() const override;
};


#endif

