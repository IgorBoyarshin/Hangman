#include <iostream>

/* #include "Display.h" */
#include "Game.h"

// For sleeping
#include <chrono>
#include <thread>


int main() {
    std::cout << "---------- Start ----------" << std::endl;

    Display display;
    start_color();
    display.drawField();

    Game game;

    // move(3,3);
    // std::string str = "Colors: " + std::to_string(COLORS) + "; Pairs: " + std::to_string(COLOR_PAIRS);
    // addstr(str.c_str());
    // move(4,4);
    // init_pair(1, 2, 2);
    // init_pair(2, 1, 1);
    //
    // move(4,4);
    // attron(COLOR_PAIR(1));
    // addstr("text");
    // refresh();
    //
    // getch();
    //
    // std::this_thread::sleep_for(std::chrono::milliseconds(130));
    // move(4,4);
    // attron(COLOR_PAIR(2));
    // addstr("text");
    // refresh();
    //
    // std::this_thread::sleep_for(std::chrono::milliseconds(130));
    // move(4,4);
    // attron(COLOR_PAIR(1));
    // addstr("text");
    // refresh();

            nodelay(stdscr, FALSE);
    getch();

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
