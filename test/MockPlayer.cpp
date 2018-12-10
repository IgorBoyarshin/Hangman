// For sleeping
#include <chrono>
#include <thread>

#include <iostream>
#include <exception>
#include <regex>

#include "../src/NetworkManager.h"
#include "../src/Communicator.h"


int main(int argc, char* argv[]) {
    const std::string name = "MockPlayer";
    const std::string addr = "172.17.0.2";
    const unsigned int port = [](int argc, char* argv[]){
        static const unsigned int DEFAULT_PORT = 8080;
        if (argc == 2) {
            unsigned int parsed_port = DEFAULT_PORT;
            try {
                parsed_port = static_cast<unsigned int>(std::stoi(std::string(argv[1])));
            } catch (std::exception& e) {
                std::cout << "Invalid argument supplied. Assuming default port" << std::endl;
            }
            return parsed_port;
        } else {
            std::cout << "No proper arguments specified. Assuming default port" << std::endl;
            return DEFAULT_PORT;
        }
    }(argc, argv);

    Communicator* communicator = new NetworkManager(name, addr, port);
    const unsigned int SLEEP_MILLIS = 1000;
    while (true) {
        std::cout << "Waiting..." << std::endl;
        if (const auto messageOpt = communicator->receive()) {
            const std::string message = *messageOpt;
            std::cout << "Received message:" << message << std::endl;
            if (std::regex_match(message, std::regex("(hello from )(.*)"))) {
                std::cout << "Recognized as \"hello\"" << std::endl;

                static const auto addrStart = 11;
                const auto addrEnd = message.find(' ', addrStart);
                const std::string opponentAddr = message.substr(addrStart, addrEnd - addrStart);
                const auto portStart = addrEnd + 1;
                const auto portEnd = (message.find(' ', portStart) == std::string::npos) ?
                    message.size() :
                    message.find(' ', portStart);
                const unsigned int opponentPort = static_cast<unsigned int>
                    (std::stoi(message.substr(portStart, portEnd - portStart)));
                std::cout << "Opponent info:" << opponentAddr << ":" << opponentPort << "." << std::endl;

                // Send reply
                communicator->send(opponentAddr, opponentPort, "hello to you too, good sir!!");
            } else {
                std::cout << "Not recognized" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLIS));
    }

    delete communicator;

    return 0;
}
