#include <iostream>

#include "version.h"
#include "Game.h"
#include "Log.h"



int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Log::info() << "Project Version = " << GLOBAL_VERSION << std::endl;
    {
        // Game game;

        // nodelay(stdscr, FALSE);
        // getch();
    }
    /* Log::error() << "df" << std::endl; */

    // Log log;
    // log.setLevel(Log::LogLevel::Info) << std::endl;

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
