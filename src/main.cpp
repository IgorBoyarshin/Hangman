#include <iostream>

#include "version.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Log.h"



int main(int argc, char* argv[]) {
    std::cout << "---------- Start ----------" << std::endl;

    Log::setDestination(Log::Destination::File);
    Log::info() << "Project Version = " << GLOBAL_VERSION << std::endl;
    {
        std::string address;
        unsigned int port;
        if (argc == 3) {
            Log::info() << "Using provided address and port" << std::endl;
            address = std::string(argv[1]);
            port = static_cast<unsigned int>(std::stoi(std::string(argv[2])));
        } else {
            Log::info() << "Assuming default address and port." << std::endl;
            address = "127.0.0.1";
            port = 3141;
        }
        const unsigned int WIDTH = 60;
        const unsigned int HEIGHT = 21;
        Drawer* drawer = new Renderer;
        Communicator* communicator = new NetworkManager("MainNM", address, port);
        Game game{WIDTH, HEIGHT, drawer, communicator};
        game.init();
        game.loop();

        delete drawer;
        delete communicator;
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
