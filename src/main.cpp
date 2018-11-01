#include <iostream>

/* #include "Display.h" */
#include "Game.h"

// For sleeping
#include <chrono>
#include <thread>


int main() {
    std::cout << "---------- Start ----------" << std::endl;

    {
        Game game;

        // nodelay(stdscr, FALSE);
        // getch();
    }

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
