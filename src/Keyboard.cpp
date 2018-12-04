#include "Keyboard.h"


void Keyboard::blockOnRead(bool doBlock) noexcept {
    if (doBlock) nodelay(stdscr, FALSE);
    else nodelay(stdscr, TRUE);
}

void Keyboard::echo(bool doEcho) noexcept {
    if (doEcho) {}
    else noecho();
}

void Keyboard::waitCarriageReturn(bool doWait) noexcept {
    if (doWait) {}
    else cbreak();
}

std::optional<Key> Keyboard::read() noexcept {
    const int c = getch();
    if (c == ERR) {
        return std::nullopt;
    } else {
        return {Key(c)};
    }
}

void Keyboard::discardCharBuffer() noexcept {
    flushinp();
}

void Keyboard::enableSpecialKeys() noexcept {
    keypad(stdscr, TRUE);
}
