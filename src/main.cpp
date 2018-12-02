#include <iostream>

#include "version.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Log.h"



int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log::inst().setDestination(Log::Destination::File);
    Log::info() << "Project Version = " << GLOBAL_VERSION << std::endl;
    {
        const unsigned int WIDTH = 20;
        const unsigned int HEIGHT = 60;
        Drawer* drawer = new Renderer;
        Communicator* communicator = new NetworkManager;
        Game game{WIDTH, HEIGHT, drawer, communicator};
        game.init();
        game.loop();

        delete drawer;
        delete communicator;
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
