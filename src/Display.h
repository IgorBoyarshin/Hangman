#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <iostream>


class Display {
    private:
        unsigned int m_Width;
        unsigned int m_Height;


        void init() noexcept;
        void cleanup() const noexcept;

    public:
        Display();
        virtual ~Display();
        /* Display(unsigned int width, unsigned int height); */

        void drawField() noexcept;
};


#endif
