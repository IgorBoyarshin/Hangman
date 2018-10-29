#include <iostream>

#include "Client.h"
#include "Server.h"
#include "Log.h"


int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log log;
    std::cout << log.setLevel(Log::LogLevel::Info) << "test" << std::endl;


    const std::string serverAddress = "192.168.1.3";
    const unsigned int serverPort = 2345;
    Server server(serverAddress, serverPort);
    Client client("MyClientName", serverAddress, serverPort);

    // server.start();
    for (int i = 0; i < 3; i++) {
        client.send("hello " + std::to_string(i));
        client.send("boobs " + std::to_string(i));

        while (server.hasMessages()) {
            std::cout << "Received: " << *(server.popMessage()) << std::endl;
        }
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
