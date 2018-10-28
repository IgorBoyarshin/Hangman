#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>


class Display {
    public:
        enum WindowType {
            Window_Network = 0,
            Window_Game,
            Window_About
        };

        class Window {
            private:
                const std::string m_Name;

            public:
        };


    private:
        unsigned int m_Width;
        unsigned int m_Height;

        // Network, Game, About
        std::vector<Window> windows;

        void init() noexcept;
        void cleanup() const noexcept;

    public:
        Display();
        virtual ~Display();

        void drawField() const noexcept;
        void drawNetworkStats(
                const std::string& selfAddress,
                const std::string& selfPort,
                const std::string& opponentAddress,
                const std::string& opponentPort) const noexcept;

        /* void handleInput() */

};


#endif
