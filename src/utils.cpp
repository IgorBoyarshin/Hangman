#include "utils.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Coord
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Coord::Coord() : Coord(0, 0) {}


Coord::Coord(int y, int x) : y(y), x(x) {}


Coord& Coord::operator+=(const Coord& coord) noexcept {
    y += coord.y;
    x += coord.x;
    return *this;
}

Coord Coord::operator+(const Coord& rhs) const noexcept {
    return {y + rhs.y, x + rhs.x};
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Color
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Color::Color(unsigned int fg, unsigned int bg) noexcept : fg(fg), bg(bg) {};

unsigned int Color::BLACK = 0;
unsigned int Color::RED = 1;
unsigned int Color::GREEN = 2;
unsigned int Color::YELLOW = 3;
unsigned int Color::BLUE = 4;
unsigned int Color::PURPLE = 5;
unsigned int Color::CYAN = 6;
unsigned int Color::WHITE = 7;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ColorFactory
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* unsigned int ColorFactory::m_NextAvailableIndex = 1; */
/* std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>> */
/*     ColorFactory::m_Colors{}; */
/*  */
/* // TODO: Test */
/* void ColorFactory::setColor(const Color& color) noexcept { */
/*     const unsigned int fg = color.fg; */
/*     const unsigned int bg = color.bg; */
/*  */
/*     if (m_Colors.find(fg) == m_Colors.end() || */
/*             m_Colors[fg].find(bg) == m_Colors[fg].end()) { */
/*         // If either doesn't exist => no such pair yet => create */
/*         init_pair(m_NextAvailableIndex, fg, bg); */
/*         if (m_Colors.find(fg) == m_Colors.end()) m_Colors[fg] = {}; */
/*         m_Colors[fg][bg] = m_NextAvailableIndex++; */
/*     } */
/*  */
/*     attron(COLOR_PAIR(m_Colors[fg][bg])); */
/* } */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tag
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const unsigned int Tag::m_EmptyTag = 0;
unsigned int Tag::m_NextAvailableIndex = Tag::m_EmptyTag + 1;

Tag::Tag(unsigned int value) noexcept : value(value) {}

bool Tag::isEmpty() const noexcept {
    return value == m_EmptyTag;
}

unsigned int Tag::operator()() const noexcept {
    return value;
}

Tag Tag::createEmpty() noexcept {
    return {m_EmptyTag};
}

Tag Tag::createNew() noexcept {
    return {m_NextAvailableIndex++};
}
