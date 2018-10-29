#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <unordered_map>
#include <cassert> // TODO: remove once all calls to assert() and replace with Log.assert()


class Display {
    public:
        struct Coord {
            public:
                // Origin: (0,0) === Upper-left corner
                int y; // row
                int x; // column

                Coord();
                Coord(int y, int x);

                Coord& operator+=(const Coord& coord) noexcept;
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
                virtual bool handleInputSymbol(int c, const Coord& coord) noexcept = 0;

                virtual void handleCursorOver() noexcept = 0;
                virtual void handleCursorAway() noexcept = 0;

                // Called when the current Window is changed or reset to signal
                // the element that it needs to return to an idle state
                virtual void unfocus() noexcept = 0;
        };


        // class Label : public Drawable {
        //
        // };

        // class Button : public Interactable {
        //
        // };

        class Field : public Drawable, public Interactable {
            private:
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };

            public:
                bool handleInputSymbol(int c, const Coord& coord) noexcept override;
                void handleCursorOver() noexcept override;
                void handleCursorAway() noexcept override;
                void unfocus() noexcept override;
                void draw() const noexcept override;

            private:
                Coord m_Coord;
                unsigned int m_Size;
                const unsigned int m_MaxSize;
                std::string m_Value;
                State m_State = State::Idle;

            public:
                // TODO: reorder to make more sense
                Field(const Coord& coord,
                        unsigned int initialSize, unsigned int maxSize,
                        const std::string& initialValue = "");

                // bool append(char c) noexcept {
                //     if (m_Size < m_MaxSize) {
                //         m_Value += c;
                //         m_Size++;
                //         return true;
                //     }
                //
                //     return false;
                // }

        };

        class Window {
            private:
                /* const std::string m_Name; */

                // std::vector<Label> m_Labels;
                // std::vector<Button> m_Buttons;
                std::vector<Field> m_Fields;

                // TODO: move outside the class
                std::string toString(Display::WindowType windowType);


            public:
                // Window(WindowType windowType) : m_Name(toString(windowType)) {
                //
                // }

                void reset() noexcept;
                void unfocus();

                Window& addLabel(const Coord& coord, std::string text);
                Window& addField(
                        const Coord& coord,
                        unsigned int initialSize, unsigned int maxSize,
                        std::string initialValue = "");
                Window& addButton(const Coord& coord, std::string name,
                                    std::function<void()> feedback);

                std::optional<std::reference_wrapper<Interactable>>
                        getInteractableUnder(const Coord& coord) noexcept;
        };


    private:
        unsigned int m_Height; // amount of rows
        unsigned int m_Width;  // amount of columns

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

        Coord getCursor() const noexcept;

        std::optional<std::reference_wrapper<Interactable>>
                getInteractableUnderCursor() noexcept;

        void setActiveWindow(WindowType windowType);

        // Resets the contents of the specified window and returns it
        Window& initWindow(WindowType windowType);

        // Returns the specified window
        Window& populateWindow(WindowType windowType);

        // Either completes completely or not at all
        bool shiftCursor(Coord shift) noexcept ;
        // Check where the cursor ends up and toggle appropriate Stateful objects
        bool setCursor(const Coord& coord) noexcept;

        void draw();

        void drawBorder() const noexcept;
};


#endif
