#include "Renderer.h"


Renderer::Renderer(unsigned int height, unsigned int width)
        : m_Height(height), m_Width(width) {}

Renderer::~Renderer() {
    cleanup();
}


void Renderer::_move(unsigned int y, unsigned int x) {
    move(y, x);
}

void Renderer::_addstr(const std::string& string) {
    addstr(string.c_str());
}

void Renderer::_flushinp() {
    flushinp();
}

void Renderer::_addch(int c) {
    addch(c);
}

void Renderer::_attron(int attr) {
    attron(attr);
}

void Renderer::_attroff(int attr) {
    attroff(attr);
}

void Renderer::_refresh() {
    refresh();
}


void Renderer::_keypad() {
    keypad(stdscr, TRUE);
}

void Renderer::_initscr() {
    initscr();
}

void Renderer::_noecho() {
    noecho();
}

void Renderer::_start_color() {
    start_color();
}

void Renderer::_erase() {
    erase();
}

void Renderer::cleanup() {
    // endwin();
}


/* const int Drawer::_ACS_ULCORNER = ACS_ULCORNER; */
/* const int Drawer::_ACS_URCORNER = ACS_URCORNER; */
/* const int Drawer::_ACS_LLCORNER = ACS_LLCORNER; */
/* const int Drawer::_ACS_LRCORNER = ACS_LRCORNER; */
/* const int Drawer::_ACS_HLINE = ACS_HLINE; */
/* const int Drawer::_ACS_VLINE = ACS_VLINE; */
/* const int Drawer::_A_BOLD = A_BOLD; */
/* const int Drawer::_A_UNDERLINE = A_UNDERLINE; */
/* const int Drawer::_KEY_DOWN = KEY_DOWN; */
/* const int Drawer::_KEY_UP = KEY_UP; */
/* const int Drawer::_KEY_LEFT = KEY_LEFT; */
/* const int Drawer::_KEY_RIGHT = KEY_RIGHT; */
/* const int Drawer::_KEY_DC = KEY_DC; */


void Renderer::setColor(const Color& color) {
    const unsigned int fg = color.fg;
    const unsigned int bg = color.bg;

    if (m_Colors.find(fg) == m_Colors.end() ||
            m_Colors[fg].find(bg) == m_Colors[fg].end()) {
        // If either doesn't exist => no such pair yet => create
        init_pair(m_NextAvailableColorIndex, fg, bg);
        if (m_Colors.find(fg) == m_Colors.end()) m_Colors[fg] = {};
        m_Colors[fg][bg] = m_NextAvailableColorIndex++;
    }

    attron(COLOR_PAIR(m_Colors[fg][bg]));
}
