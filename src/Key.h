#ifndef KEY_H
#define KEY_H

#include <variant>
#include <optional>
#include <ncurses.h>
#include "Log.h"


enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Key {
    public:
        enum class SpecialKey {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            ENTER,
            DELETE,
            BACKSPACE
        };

    public:
        std::variant<char, SpecialKey> key;

    public:
        Key(int c) noexcept;
        bool isRegular() const noexcept;
        bool isSpecial() const noexcept;
        std::optional<char> getRegular() const noexcept;
        std::optional<SpecialKey> getSpecial() const noexcept;
        bool isLetter() const noexcept;
        bool isNumber() const noexcept;
        bool operator==(const Key& other) const noexcept;
        bool is(char c) const noexcept;
        bool is(SpecialKey sk) const noexcept;
        std::optional<Direction> asDirection() const noexcept;
    private:
        static std::optional<SpecialKey> toSpecialKey(int c) noexcept;
        static std::variant<char, SpecialKey> toCharVariant(int c) noexcept;
};


#endif
