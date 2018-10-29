#ifndef GAME_H
#define GAME_H

#include "Display.h"

// For sleeping
#include <chrono>
#include <thread>


inline bool isStandartChar(int c) {
    return (c == static_cast<char>(c));
}

inline bool isExtendedChar(int c) {
    return !isStandartChar(c);
}

bool isMovementChar(int c);

// TODO: TEST
bool isUpperCase(int c);


class Game {
    private:
        Display m_Display;

        void init();
        void initDisplay();
        void cleanup();
        void loop();
        /* void quit(); */
        void handleInput();
        // TODO: Tests here
        void processInputSymbol(int c);

    public:
        Game();
        virtual ~Game();
};



#endif
