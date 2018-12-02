#ifndef DRAWER_H
#define DRAWER_H

#include "utils.h"
#include <ncurses.h>
#include <iostream>
#include "Log.h"


class Drawer {
    public:
        virtual ~Drawer() {}
        // TODO: change into more abstract ones
        virtual void _move(unsigned int y, unsigned int x) = 0;
        virtual void _addstr(const std::string& string) = 0;
        virtual void _flushinp() = 0;
        virtual void _addch(int c) = 0;
        virtual void _attron(int attr) = 0;
        virtual void _attroff(int attr) = 0;
        virtual void _refresh() = 0;
        virtual void _keypad() = 0;
        virtual void _initscr() = 0;
        virtual void _noecho() = 0;
        virtual void _start_color() = 0;
        virtual void _erase() = 0;

        virtual void setColor(const Color& color) = 0;

        // virtual void draw() = 0;
        virtual void cleanup() = 0;
        // virtual void setup() = 0;
        // virtual void rectangle(const Coord& upperLeft, const Coord& bottomRight) = 0;

    public:
        /* static const int _ACS_ULCORNER; */
        /* static const int _ACS_URCORNER; */
        /* static const int _ACS_LLCORNER; */
        /* static const int _ACS_LRCORNER; */
        /* static const int _ACS_HLINE; */
        /* static const int _ACS_VLINE; */
        /* static const int _A_BOLD; */
        /* static const int _A_UNDERLINE; */
        /* static const int _KEY_DOWN; */
        /* static const int _KEY_UP; */
        /* static const int _KEY_LEFT; */
        /* static const int _KEY_RIGHT; */
        /* static const int _KEY_DC; */
};


#endif
