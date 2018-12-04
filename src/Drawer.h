#ifndef DRAWER_H
#define DRAWER_H

#include <iostream>
#include "utils.h"
#include "Log.h"


class Drawer {
    public:
        enum class SpecialChar {
            ULCORNER,
            URCORNER,
            LLCORNER,
            LRCORNER,
            HLINE,
            VLINE,
            LTEE,
            RTEE,
            BTEE,
            TTEE,
            PLUS,
            S1,
            S3,
            S7,
            S9,
            DIAMOND,
            CKBOARD,
            DEGREE,
            PLMINUS,
            BULLET,
            LARROW,
            RARROW,
            DARROW,
            UARROW,
            BOARD,
            LANTERN,
            BLOCK,
            LEQUAL,
            GEQUAL,
            PI,
            NEQUAL,
            STERLING
        };

        enum class Attribute {
            BOLD,
            UNDERLINE
        };

        virtual ~Drawer() {}
        virtual void goTo(unsigned int y, unsigned int x) = 0;
        virtual void put(const std::string& string) = 0;
        virtual void put(char c) = 0;
        virtual void put(SpecialChar specialChar) = 0;
        virtual void setAttribute(Attribute attribute, bool on) = 0;
        virtual void update() = 0;
        virtual void init() = 0;
        virtual void enableColors() = 0;
        virtual void clearScreen() = 0;
        virtual void setColor(const Color& color) = 0;
    private:
        virtual void cleanup() = 0;
};


#endif
