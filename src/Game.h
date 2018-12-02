#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "Display.h"
#include "Renderer.h"
#include "Communicator.h"

// For sleeping
#include <chrono>
#include <thread>


bool isStandartChar(int c);
bool isExtendedChar(int c);
bool isMovementChar(int c);
bool isUpperCase(int c);


class Game {
    private:
        Display m_Display;
        Communicator* m_Communicator;
        bool m_Terminated;

    public:
        Game(unsigned int width, unsigned int height,
                Drawer* drawer, Communicator* communicator);
        virtual ~Game();
        void init();
        void loop();
    private:
        void initDisplay();
        void cleanup();
        void handleInput();
        void processInputSymbol(int c);
};


#endif
