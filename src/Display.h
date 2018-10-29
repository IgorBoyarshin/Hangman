#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <cassert> // TODO: remove once all calls to assert() have been replaced with Log.assert()


class Display {
    public:
        struct Coord {
            public:
                // Origin: (0,0) === Upper-left corner
                int y; // row
                int x; // column

                Coord() : y(0), x(0) {}
                Coord(int y, int x) : y(y), x(x) {}

                Coord& operator+=(const Coord& coord) noexcept {
                    y += coord.y;
                    x += coord.x;
                    return *this;
                }
        };

        enum class WindowType {
            Settings,
            Game,
            About
        };

        class Drawable {
            public:
                virtual void draw() const noexcept = 0;
        };

        class Interactable {
            public:
                // Returns whether the input was handled. If it wasn't => may
                // need to handle it in the outter layer
                virtual bool handleInputSymbol() noexcept = 0;

                virtual void handleCursorOver() noexcept = 0;
                virtual void handleCursorAray() noexcept = 0;

                // Called when the current Window is changed or reset to signal
                // the element that it needs to return to an idle state
                virtual void unfocus() noexcept = 0;
        };


        class Label {

        };

        class Button : public Interactable {

        };

        class Field : public Drawable, public Interactable {
            private:
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };

                State m_State = State::Idle; // TODO: into constructor

            public:
                Coord coord;

            public:
                bool handleInputSymbol() noexcept override {
                    return true;
                }
                void handleCursorOver() noexcept override {
                    if (m_State == State::Idle) {
                        m_State = State::Highlighted;
                        draw();
                    }
                }
                void handleCursorAray() noexcept override {
                    if (m_State == State::Highlighted) {
                        m_State = State::Idle;
                        draw();
                    }
                }
                void unfocus() noexcept override {
                    m_State = State::Idle;
                    // TODO: check that the field is OK
                }

            private:
                unsigned int m_Size;
                const unsigned int m_MaxSize;
                std::string m_Value;

                void draw() const noexcept override {
                    // TODO
                    refresh();
                }

            public:
                Field(const Coord& coord,
                        unsigned int initialSize, unsigned int maxSize,
                        const std::string& initialValue = "")
                        : coord(coord), m_Size(initialSize),
                          m_MaxSize(maxSize), m_Value(initialValue) {}

                bool append(char c) noexcept {
                    if (m_Size < m_MaxSize) {
                        m_Value += c;
                        m_Size++;
                        return true;
                    }

                    return false;
                }

        };

        class Window {
            private:
                /* const std::string m_Name; */

                std::vector<Label> m_Labels;
                std::vector<Button> m_Buttons;
                std::vector<Field> m_Fields;

                // TODO: move outside the class
                std::string toString(Display::WindowType windowType) {
                    switch (windowType) {
                        case Display::WindowType::Settings:
                            return "Settings";
                        case Display::WindowType::Game:
                            return "Game";
                        case Display::WindowType::About:
                            return "About";
                        default:
                            // TODO: Log.asert(false);
                            // TODO: maybe throw??
                            return "";
                    }
                }


            public:
                // Window(WindowType windowType) : m_Name(toString(windowType)) {
                //
                // }

                void reset() noexcept {
                    // TODO: check that the order corresponds to the order of declaration
                    m_Labels.clear();
                    m_Buttons.clear();
                    m_Fields.clear();
                }

                Window& addLabel(const Coord& coord, std::string text) {
                    // TODO
                    return *this;
                }
                Window& addField(
                        const Coord& coord,
                        unsigned int initialSize, unsigned int maxSize,
                        std::string initialValue = "") {
                    // TODO
                    return *this;
                }
                Window& addButton(const Coord& coord, std::string name,
                        std::function<void()> feedback) {
                    // TODO
                    return *this;
                }

                // Goes through all the elements and calls makeIdle() on them
                void makeAllIdle() {
                    for (Stateful& stateful : m_Buttons) {
                        stateful.makeIdle();
                    }
                    for (Stateful& stateful : m_Fields) {
                        stateful.makeIdle();
                    }
                }
        };


    private:
        unsigned int m_Height; // amount of rows
        unsigned int m_Width;  // amount of columns

        // Network, Game, About
        std::unordered_map<WindowType, Window> m_Windows;
        WindowType m_ActiveWindowType;

        Coord m_Cursor;

        inline bool inbounds(const Coord& coord) const noexcept {
            return (0 <= coord.y) && (coord.y <= static_cast<int>(m_Height))
                && (0 <= coord.x) && (coord.x <= static_cast<int>(m_Width));
        }


        void init() noexcept;
        void cleanup() const noexcept;

    public:
        Display();
        virtual ~Display();

        Coord getCursor() const noexcept {
            return m_Cursor;
        }

        void setActiveWindow(WindowType windowType) {
            if (m_ActiveWindowType == windowType) return;
            m_Windows[m_ActiveWindowType].makeAllIdle();
            m_ActiveWindowType = windowType;
            setCursor({0, 0}); // default cursor position in new window
        }

        // Resets the contents of the specified window and returns it
        Window& initWindow(WindowType windowType) {
            m_Windows[windowType].reset();
            return m_Windows[windowType];
        }

        // Returns the specified window
        Window& populateWindow(WindowType windowType) {
            return m_Windows[windowType];
        }

        // Either completes completely or not at all
        bool shiftCursor(Coord shift) noexcept {
            return setCursor(shift += m_Cursor); // attention: not the other way around
            /* return setCursor({m_Cursor.y + shift.y, m_Cursor.x + shift.x}); */
        }

        // Check where the cursor ends up and toggle appropriate Stateful objects
        bool setCursor(const Coord& coord) noexcept {
            if (inbounds(coord)) {
                m_Cursor = coord;
                move(m_Cursor.y, m_Cursor.x);
                return true;
            }
            return false;
            /* if (inbounds(coord)) { */
            /*  */
            /*  */
            /*     m_Cursor = coord; */
            /*  */
            /*     move(m_Cursor.y, m_Cursor.x); */
            /*     Element& element = getElementUnder(m_Cursor); */
            /*     refresh(); */
            /*     return true; */
            /* } */
            /*  */
            /* return false; */
        }

        void draw() {
            drawBorder();
        }

        void drawBorder() const noexcept;
};



        /* enum class State { */
        /*     Idle, */
        /*     Highlighted, */
        /*     Focused */
        /* }; */
        /*  */
        /* class Stateful { */
        /*     private: */
        /*         State m_State; */
        /*  */
        /*     public: */
        /*         Stateful() : m_State(State::Idle) {} */
        /*  */
        /*         virtual bool isUnder(const Coord& coord) const noexcept = 0; */
        /*  */
        /*         virtual void handleCursorOver() noexcept = 0; */
        /*         virtual void handleCursorAway() noexcept = 0; */
        /*  */
        /*         // TODO: check if needed */
        /*         virtual void makeIdle() noexcept = 0; */
        /*         #<{(| void makeIdle() noexcept { |)}># */
        /*         #<{(|     if (m_State == State::Focused) { |)}># */
        /*         #<{(|         // TODO: replace with Log.assert |)}># */
        /*         #<{(|         // Why would you call makeIdle() from this state?? |)}># */
        /*         #<{(|         assert(false && "Called makeIdle() from State::Focused."); |)}># */
        /*         #<{(|     } |)}># */
        /*         #<{(|     m_State = State::Idle; |)}># */
        /*         #<{(|     draw(); |)}># */
        /*         #<{(| } |)}># */
        /*  */
        /*  */
        /*  */
        /*         // TODO: check if needed */
        /*         bool idle() const noexcept { */
        /*             return m_State == State::Idle; */
        /*         } */
        /*  */
        /*         // TODO: check if needed */
        /*         bool highlighted() const noexcept { */
        /*             return m_State == State::Highlighted; */
        /*         } */
        /*  */
        /*         // TODO: check if needed */
        /*         bool focused() const noexcept { */
        /*             return m_State == State::Focused; */
        /*         } */
        /*  */
        /*  */
        /*  */
        /*  */
        /*         virtual void focus() noexcept = 0; */
        /*         #<{(| void focus() noexcept { |)}># */
        /*         #<{(|     // TODO: replace with Log.assert |)}># */
        /*         #<{(|     // Don't let it be Focus upon call to prevent weird cases |)}># */
        /*         #<{(|     assert(m_State == State::Highlighted); |)}># */
        /*         #<{(|  |)}># */
        /*         #<{(|     m_Focused = true; |)}># */
        /*         #<{(|     draw(); |)}># */
        /*         #<{(| } |)}># */
        /*  */
        /*         #<{(| void unfocus() noexcept { |)}># */
        /*         #<{(|     // TODO: replace with Log.assert |)}># */
        /*         #<{(|  |)}># */
        /*         #<{(|     if (m_State == m_Focused) { |)}># */
        /*         #<{(|         m_State = State::Highlighted; |)}># */
        /*         #<{(|     } |)}># */
        /*         #<{(|     draw(); |)}># */
        /*         #<{(| } |)}># */
        /* }; */






#endif
