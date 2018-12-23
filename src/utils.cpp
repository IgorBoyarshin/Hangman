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
unsigned int Color::BLACK = 0;
unsigned int Color::RED = 1;
unsigned int Color::GREEN = 2;
unsigned int Color::YELLOW = 3;
unsigned int Color::BLUE = 4;
unsigned int Color::PURPLE = 5;
unsigned int Color::CYAN = 6;
unsigned int Color::WHITE = 7;

Color::Color(unsigned int fg, unsigned int bg) noexcept
    : fg(fg), bg(bg) {};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tag
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const unsigned int Tag::m_EmptyTag = 0;
unsigned int Tag::m_NextAvailableIndex = Tag::m_EmptyTag + 1;

Tag::Tag(unsigned int value) noexcept : value(value) {}

unsigned int Tag::operator()() const noexcept {
return value;
}

bool Tag::operator==(const Tag& other) const noexcept {
    return this->is(other);
}

bool Tag::is(const Tag& other) const noexcept {
    return other.value == this->value;
}

bool Tag::isEmpty() const noexcept {
    return value == m_EmptyTag;
}

Tag Tag::createEmpty() noexcept {
    return {m_EmptyTag};
}

Tag Tag::createNew() noexcept {
    return {m_NextAvailableIndex++};
}
