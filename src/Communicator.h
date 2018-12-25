#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <optional>
#include <string>


class Communicator {
    public:
        virtual ~Communicator() {}
        virtual bool establishConnection(const std::string& ip, const std::string& port) = 0;
        virtual bool connectionEstablished() = 0;

        virtual std::optional<std::string> receive() = 0;
        virtual std::string blockReceive() = 0;
        virtual bool send(const std::string& addr, unsigned int port, const std::string& message) = 0;
        virtual void sendAsync(const std::string& addr, unsigned int port, const std::string& message) = 0;

        virtual std::string getAddress() const = 0;
        virtual unsigned int getPort() const = 0;
};


#endif
