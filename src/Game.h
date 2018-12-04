#ifndef GAME_H
#define GAME_H

// For sleeping
#include <chrono>
#include <thread>

#include "Display.h"
#include "Renderer.h"
#include "Communicator.h"
#include "Keyboard.h"
#include "Key.h"


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
        void processInputKey(Key key);
};


#endif
