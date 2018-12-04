#ifndef DRAWER_H
#define DRAWER_H

#include "utils.h"
/* #include <ncurses.h> */
#include <iostream>
#include "Log.h"


class Drawer {
    public:
        enum class SpecialChar {
            ULCORNER,
            URCORNER,
            LLCORNER,
            LRCORNER,
            HLINE,
            VLINE
        };

        enum class Attribute {
            BOLD,
            UNDERLINE
        };

        virtual ~Drawer() {}
        // virtual void _flushinp() = 0;
        virtual void _keypad() = 0;
        virtual void _initscr() = 0;
        virtual void _noecho() = 0;
        virtual void _start_color() = 0;
        virtual void _erase() = 0;

        virtual void goTo(unsigned int y, unsigned int x) = 0;
        virtual void putstr(const std::string& string) = 0;
        virtual void putch(char c) = 0;
        virtual void putsch(SpecialChar specialChar) = 0;
        virtual void setAttribute(Attribute attribute, bool on) = 0;
        virtual void update() = 0;

        virtual void setColor(const Color& color) = 0;
        virtual void cleanup() = 0;

        // virtual void draw() = 0;
        // virtual void setup() = 0;
        // virtual void rectangle(const Coord& upperLeft, const Coord& bottomRight) = 0;
};


#endif
