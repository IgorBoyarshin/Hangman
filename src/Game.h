#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "Display.h"
#include "Renderer.h"
#include "Communicator.h"

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
        Communicator* m_Communicator;

        void initDisplay();
        void cleanup();
        /* void quit(); */
        void handleInput();
        void processInputSymbol(int c); // TODO: Tests here

        bool m_Terminated;

    public:
        Game(Drawer* drawer, Communicator* communicator);
        virtual ~Game();

        void init();
        void loop();

        bool connect(const std::string& ip, const std::string port);
        bool send(const std::string& message);
        std::optional<std::string> receive();
};



#endif
