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
        case SpecialChar::LTEE:
            addch(ACS_LTEE);
            break;
        case SpecialChar::RTEE:
            addch(ACS_RTEE);
            break;
        case SpecialChar::BTEE:
            addch(ACS_BTEE);
            break;
        case SpecialChar::TTEE:
            addch(ACS_TTEE);
            break;
        case SpecialChar::PLUS:
            addch(ACS_PLUS);
            break;
        case SpecialChar::S1:
            addch(ACS_S1);
            break;
        case SpecialChar::S3:
            addch(ACS_S3);
            break;
        case SpecialChar::S7:
            addch(ACS_S7);
            break;
        case SpecialChar::S9:
            addch(ACS_S9);
            break;
        case SpecialChar::DIAMOND:
            addch(ACS_DIAMOND);
            break;
        case SpecialChar::CKBOARD:
            addch(ACS_CKBOARD);
            break;
        case SpecialChar::DEGREE:
            addch(ACS_DEGREE);
            break;
        case SpecialChar::PLMINUS:
            addch(ACS_PLMINUS);
            break;
        case SpecialChar::BULLET:
            addch(ACS_BULLET);
            break;
        case SpecialChar::LARROW:
            addch(ACS_LARROW);
            break;
        case SpecialChar::RARROW:
            addch(ACS_RARROW);
            break;
        case SpecialChar::DARROW:
            addch(ACS_DARROW);
            break;
        case SpecialChar::UARROW:
            addch(ACS_UARROW);
            break;
        case SpecialChar::BOARD:
            addch(ACS_BOARD);
            break;
        case SpecialChar::LANTERN:
            addch(ACS_LANTERN);
            break;
        case SpecialChar::BLOCK:
            addch(ACS_BLOCK);
            break;
        case SpecialChar::LEQUAL:
            addch(ACS_LEQUAL);
            break;
        case SpecialChar::GEQUAL:
            addch(ACS_GEQUAL);
            break;
        case SpecialChar::PI:
            addch(ACS_PI);
            break;
        case SpecialChar::NEQUAL:
            addch(ACS_NEQUAL);
            break;
        case SpecialChar::STERLING:
            addch(ACS_STERLING);
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
