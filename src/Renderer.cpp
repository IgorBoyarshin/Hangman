#include "Renderer.h"


Renderer::Renderer() {}


Renderer::~Renderer() {
    cleanup();
}


void Renderer::goTo(unsigned int y, unsigned int x) {
    move(y, x);
}

void Renderer::putstr(const std::string& string) {
    addstr(string.c_str());
}

void Renderer::putch(char c) {
    addch(c);
}

void Renderer::putsch(SpecialChar specialChar) {
    switch (specialChar) {
        case SpecialChar::ULCORNER:
            addch(ACS_ULCORNER);
            break;
        case SpecialChar::URCORNER:
            addch(ACS_URCORNER);
            break;
        case SpecialChar::LLCORNER:
            addch(ACS_LLCORNER);
            break;
        case SpecialChar::LRCORNER:
            addch(ACS_LRCORNER);
            break;
        case SpecialChar::HLINE:
            addch(ACS_HLINE);
            break;
        case SpecialChar::VLINE:
            addch(ACS_VLINE);
            break;
        default:
            break;
            // TODO: error
    }
}

void Renderer::setAttribute(Attribute attribute, bool on) {
    if (on) {
        switch (attribute) {
            case Attribute::BOLD:
                attron(A_BOLD);
                break;
            case Attribute::UNDERLINE:
                attron(A_UNDERLINE);
                break;
            default:
                break;
                // TODO: error
        }
    } else {
        switch (attribute) {
            case Attribute::BOLD:
                attroff(A_BOLD);
                break;
            case Attribute::UNDERLINE:
                attroff(A_UNDERLINE);
                break;
            default:
                break;
                // TODO: error
        }
    }
}

void Renderer::update() {
    refresh();
}


/* void Renderer::_addstr(const std::string& string) { */
/*     addstr(string.c_str()); */
/* } */
/*  */
void Renderer::_flushinp() {
    flushinp();
}
/*  */
/* void Renderer::_addch(int c) { */
/*     addch(c); */
/* } */
/*  */
/* void Renderer::_attron(int attr) { */
/*     attron(attr); */
/* } */
/*  */
/* void Renderer::_attroff(int attr) { */
/*     attroff(attr); */
/* } */

/* void Renderer::_refresh() { */
/*     refresh(); */
/* } */


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
    endwin();
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
