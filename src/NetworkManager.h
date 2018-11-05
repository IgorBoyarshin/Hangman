#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Communicator.h"


// TODO
class NetworkManager : public Communicator {
    public:
        virtual ~NetworkManager();
        std::optional<std::string> receiveMessage() override;
        bool sendMessage(const std::string& message) override;
        bool establishConnection(const std::string& ip, const std::string& port) override;
        bool connectionEstablished() override;
};


#endif

