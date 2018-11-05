#include <iostream>

#include "version.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Log.h"



int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log::info() << "Project Version = " << GLOBAL_VERSION << std::endl;
    {
        Game game{new NetworkManager};
        game.init();
        game.loop();
 
        // nodelay(stdscr, FALSE);
        // getch();
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
