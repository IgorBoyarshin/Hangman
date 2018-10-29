#ifndef GAME_H
#define GAME_H

#include "Display.h"

/* // For sleeping */
/* #include <chrono> */
/* #include <thread> */


inline bool isStandartChar(int c) {
    return (c == static_cast<char>(c));
}

inline bool isExtendedChar(int c) {
    return !isStandartChar(c);
}

// TODO: TEST
bool isUpperCase(int c) {
    if (isStandartChar(c)) {
        return ('A' <= c) && (c <= 'Z');
    }

    // Otherwise extended char => can't determine
    return false;
}


class Game {
    private:
        // class Context {
        //     public:
        // };

        Display m_Display;
        // Context m_Context;

        void init() {
            noecho();
            cbreak(); // no buffering (i.e. no waiting for carriage return)
            // handleInput();

            initDisplay();
            m_Display.setActiveWindow(Display::WindowType::Game);
            m_Display.draw();
        }

        void initDisplay() {
            m_Display.initWindow(Display::WindowType::Game)
                .addButton({3, 5}, "Button1", [](){/*feedback func*/})
                .addLabel({4, 2}, "Text");
        }

        void quit() {
            // TODO: need to finish Client/Server??
            // TODO: Log: quitting by user
            exit(0);
        }

        void handleInput() {
            nodelay(stdscr, TRUE); // don't block on getch()

            int c = getch();
            while (c != ERR) {
                processInputSymbol(c);
                c = getch();
            }
        }

        // TODO: Tests here
        void processInputSymbol(int c) {
            if (const std::optional<Display::Stateful> statefulUnderCursorOpt = display.underCursor();
                    statefulUnderCursorOpt) {
                // Have some Stateful under cursor (Field or Button)
                if (statefulUnderCursorOpt->isField()) {
                    // If we're over Stateful => it can't be Idle
                    // TODO: assert it
                    if (statefulUnderCursorOpt->highlighted() && c == KEY_ENTER) {
                        statefulUnderCursorOpt->focus();
                    } else if (statefulUnderCursorOpt->focused()) {
                        if (c == KEY_ENTER) {
                            statefulUnderCursorOpt->doAction();
                        } else {
                            Field& field = static_cast<Field>(*statefulUnderCursorOpt);
                            if (c == KEY_BACKSPACE) {
                                field.removeBeforeCursor(display.getCursor());
                            } else if (c == KEY_DC) {
                                field.removeUnderCursor(display.getCursor());
                            } else if (isStandartChar(c)) {
                                field.insertUnderCursor(c);
                            }
                            // Else => don't know how to handle it => do nothing
                        }
                    }
                    return;
                } else if (statefulUnderCursorOpt->isButton()) {
                    // If we're over Stateful => it can't be Idle
                    // TODO: assert it
                    // A button is never Focused, mb assert it
                    if (statefulUnderCursorOpt->)
                    Button& button = static_cast<Button>(*statefulUnderCursorOpt);
                } else {
                    assert(false && "Unhandled stateful");
                    // TODO: replace with Log.assert()
                }
            }




            if (isUpperCase(c)) {
                // TODO: process shortcut, expect when writing a message
                return;
            }

            if (c == 'q') {
                quit();
            }

            if (insideField) {
                if (field.focused()) {
                    if (symbol) {
                        focusedfield.tryAppend(c);
                    } else if (ENTER) {
                        field.unfocus();
                    } else if (ESC) {
                        // TODO: field.cancel()
                    }
                } else { // field not focused
                    if (ENTER) {
                        field.focus();
                    }
                }
                return;
            }

            if (insideButton) {

            }

            // Not inside anything interactable or not applicable to them

            if (c == 'j' || c == KEY_DOWN) {
                m_Display.shiftCursor({-1, 0});
            } else if (c == 'k' || c == KEY_UP) {
                m_Display.shiftCursor({+1, 0});
            } else if (c == 'h' || c == KEY_LEFT) {
                m_Display.shiftCursor({0, -1});
            } else if (c == 'l' || c == KEY_RIGHT) {
                m_Display.shiftCursor({0, +1});
            }

        }

    public:
        Game() {
            init();
        }
        virtual ~Game() {

        }
};



#endif
