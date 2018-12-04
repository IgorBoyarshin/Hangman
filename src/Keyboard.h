#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ncurses.h>
#include <variant>
#include <optional>
#include "Key.h"


class Keyboard {
    public:
        static void blockOnRead(bool doBlock) noexcept;
        static void echo(bool doEcho) noexcept;
        static void waitCarriageReturn(bool doWait) noexcept;
        static std::optional<Key> read() noexcept;
};


#endif
