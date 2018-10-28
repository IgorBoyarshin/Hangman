#include "Display.h"


Display::Display() : m_Width(0), m_Height(0) {
    init();
}
/* Display::Display(unsigned int width, unsigned int height)  */
/*         : m_Width(width), m_Height(height) { */
/*     init(); */
/* } */


Display::~Display() {
    cleanup();
}

void Display::init() noexcept {
    initscr();
    noecho();
    getmaxyx(stdscr, m_Height, m_Width);
    refresh();
}


void Display::cleanup() const noexcept {
    endwin();
}


void Display::drawField() const noexcept {
    static const chtype HORIZONTAL_BORDER = ACS_HLINE;
    static const chtype VERTICAL_BORDER = ACS_VLINE;
    static const chtype CORNER_LEFT_UP = ACS_ULCORNER;
    static const chtype CORNER_LEFT_DOWN = ACS_LLCORNER;
    static const chtype CORNER_RIGHT_UP = ACS_URCORNER;
    static const chtype CORNER_RIGHT_DOWN = ACS_LRCORNER;

    border(VERTICAL_BORDER, VERTICAL_BORDER, HORIZONTAL_BORDER, HORIZONTAL_BORDER,
            CORNER_LEFT_UP, CORNER_RIGHT_UP, CORNER_LEFT_DOWN, CORNER_RIGHT_DOWN);

    /* move(0, 0); */
    /* addch(CORNER_LEFT_UP); */
    /* for (unsigned int x = 1; x < m_Width - 1; x++) { */
    /*     addch(HORIZONTAL_BORDER); */
    /* } */
    /* addch(CORNER_RIGHT_UP); */
    /*  */
    /* for (unsigned int y = 1; y < m_Height - 1; y++) { */
    /*     mvaddch(y, 0, VERTICAL_BORDER); */
    /* } */
    /*  */
    /* for (unsigned int y = 1; y < m_Height - 1; y++) { */
    /*     mvaddch(y, m_Width - 1, VERTICAL_BORDER); */
    /* } */
    /*  */
    /* move(m_Height - 1, 0); */
    /* addch(CORNER_LEFT_DOWN); */
    /* for (unsigned int x = 1; x < m_Width - 1; x++) { */
    /*     addch(HORIZONTAL_BORDER); */
    /* } */
    /* addch(CORNER_RIGHT_DOWN); */
}
