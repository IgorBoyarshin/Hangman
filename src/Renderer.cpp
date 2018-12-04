#include "Renderer.h"


Renderer::Renderer() {}


Renderer::~Renderer() {
    cleanup();
}


void Renderer::goTo(unsigned int y, unsigned int x) {
    move(y, x);
}

void Renderer::put(const std::string& string) {
    addstr(string.c_str());
}

void Renderer::put(char c) {
    addch(c);
}

void Renderer::put(SpecialChar specialChar) {
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

void Renderer::init() {
    initscr();
}

void Renderer::enableColors() {
    start_color();
}

void Renderer::clearScreen() {
    erase();
}

void Renderer::cleanup() {
    endwin();
}


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
