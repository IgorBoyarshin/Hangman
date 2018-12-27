#include "Key.h"


Key::Key(int c) noexcept
        : key(toCharVariant(c)) {}

bool Key::isRegular() const noexcept {
    return std::holds_alternative<char>(key);
}

bool Key::isSpecial() const noexcept {
    return std::holds_alternative<SpecialKey>(key);
}

std::optional<char> Key::getRegular() const noexcept {
    if (isRegular()) return std::get<char>(key);
    else return std::nullopt;
}

std::optional<Key::SpecialKey> Key::getSpecial() const noexcept {
    if (isSpecial()) return std::get<SpecialKey>(key);
    else return std::nullopt;
}

bool Key::isLetter() const noexcept {
    if (!isRegular()) return false;
    const char c = *getRegular();
    return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z'));
}

bool Key::isNumber() const noexcept {
    if (!isRegular()) return false;
    const char c = *getRegular();
    return ('0' <= c) && (c <= '9');
}

bool Key::isPunctuation() const noexcept {
    if (!isRegular()) return false;
    const char c = *getRegular();
    return (c == '.') || (c == ',') || (c == '!') || (c == '?');
}

bool Key::operator==(const Key& other) const noexcept {
    if (other.isRegular() && isRegular()) {
        return *other.getRegular() == *getRegular();
    } else if (other.isSpecial() && isSpecial()) {
        return *other.getSpecial() == *getSpecial();
    } else return false;
}

bool Key::is(char c) const noexcept {
    return isRegular() && (*getRegular() == c);
}

bool Key::is(SpecialKey sk) const noexcept {
    return isSpecial() && (*getSpecial() == sk);
}

std::optional<Direction> Key::asDirection() const noexcept {
    if (isRegular()) {
        const char c = *getRegular();
        if (c == 'j') return Direction::DOWN;
        else if (c == 'k') return Direction::UP;
        else if (c == 'h') return Direction::LEFT;
        else if (c == 'l') return Direction::RIGHT;
        else return std::nullopt;
    } else {
        const SpecialKey sk = *getSpecial();
        switch (sk) {
            case SpecialKey::DOWN: return Direction::DOWN;
            case SpecialKey::UP: return Direction::UP;
            case SpecialKey::LEFT: return Direction::LEFT;
            case SpecialKey::RIGHT: return Direction::RIGHT;
            default: return std::nullopt;
        }
    }
}

std::optional<Key::SpecialKey> Key::toSpecialKey(int c) noexcept {
    static const int ENTER = 10; // Not correctly handled by ncurses, have to do like this
    static const int BACKSPACE = 127; // Not correctly handled by ncurses, have to do like this
    if (c == KEY_DOWN)       return SpecialKey::DOWN;
    else if (c == KEY_UP)    return SpecialKey::UP;
    else if (c == KEY_RIGHT) return SpecialKey::RIGHT;
    else if (c == KEY_LEFT)  return SpecialKey::LEFT;
    else if (c == ENTER)     return SpecialKey::ENTER;
    else if (c == BACKSPACE) return SpecialKey::BACKSPACE;
    else if (c == KEY_DC)    return SpecialKey::DELETE;
    else                     return std::nullopt;
}

std::variant<char, Key::SpecialKey> Key::toCharVariant(int c) noexcept {
    const auto special = toSpecialKey(c);
    if (special) return *special;
    else return static_cast<char>(c);
}
