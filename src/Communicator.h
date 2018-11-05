#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <optional>
#include <string>


class Communicator {
    public:
        virtual ~Communicator() {}
        virtual std::optional<std::string> receiveMessage() = 0;
        virtual bool sendMessage(const std::string& message) = 0;
        virtual bool establishConnection(const std::string& ip, const std::string& port) = 0;
        virtual bool connectionEstablished() = 0;
};


#endif
