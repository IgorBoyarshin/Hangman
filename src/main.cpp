#include <iostream>

#include "version.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Log.h"



int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log::info() << "Project Version = " << GLOBAL_VERSION << std::endl;
    {
        // Renderer* renderer = new Renderer(20, 60);
        // NetworkManager* networkManager = new NetworkManager;
        // Game game{renderer, networkManager};
        // game.init();
        // game.loop();
        // endwin();
 
        // delete renderer;
        // delete networkManager;
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
