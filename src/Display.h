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

// For sleeping
#include <chrono>
#include <thread>


inline bool isLetter(char c) noexcept {
    return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z'));
}
inline bool isNumber(char c) noexcept {
    return ('0' <= c) && (c <= '9');
}


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
                Coord operator+(const Coord& coord) const noexcept;
        };

        struct Color {
            public:
                unsigned int fg;
                unsigned int bg;

                Color(unsigned int fg, unsigned int bg) noexcept;

                // TODO: replace with a more elegant solution
                static unsigned int BLACK;
                static unsigned int RED;
                static unsigned int GREEN;
                static unsigned int YELLOW;
                static unsigned int BLUE;
                static unsigned int PURPLE;
                static unsigned int CYAN;
                static unsigned int WHITE;
        };

        enum class WindowType {
            Settings,
            Game,
            About
        };

        // TODO: rename
        class ColorFactory {
            public:
                static void setColor(const Color& color) noexcept;

            private:
                static unsigned int m_NextAvailableIndex;
                // [FG][BG] = ColorPair
                static std::unordered_map<unsigned int,
                    std::unordered_map<unsigned int, unsigned int>> m_Colors;
        };

        struct Tag {
            private:
                static const unsigned int m_EmptyTag;
                static unsigned int m_NextAvailableIndex;

            public:
                const unsigned int value;

                unsigned int operator()() const noexcept;
                bool isEmpty() const noexcept;

                static Tag createEmpty() noexcept;
                static Tag createNew() noexcept;

            private:
                Tag(unsigned int value) noexcept;
        };

        class UiElement {
            protected:
                Coord m_Position;
                const Tag m_Tag;

                UiElement(const Coord& position, const Tag& tag) noexcept;

            public:
                virtual void draw() const noexcept = 0;
                Tag getTag() const noexcept;
        };

        class Interactable {
            public:
                // Returns whether the input was handled. If it wasn't => may
                // need to handle it in the outter layer
                virtual bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept = 0;

                virtual void handleCursorOver() noexcept = 0;
                virtual void handleCursorAway() noexcept = 0;

                // Called when the current Window is changed or reset to signal
                // the element that it needs to return to an idle state
                virtual void unfocus() noexcept = 0;

                virtual bool isUnder(const Coord& coord) const noexcept = 0;
        };


        class Label : public UiElement {
            private:
                std::string m_Value;

            public:
                void draw() const noexcept override;

            public:
                Label(const Coord& position, const Tag& tag, const std::string& value = "") noexcept;
        };

        class Button : public UiElement, public Interactable {
            private:
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };

                const std::string m_Value;
                State m_State;
                std::function<void()> m_Feedback;

            private:
                void setColorByState() const noexcept;

            public:
                bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept override;
                void handleCursorOver() noexcept override;
                void handleCursorAway() noexcept override;
                void unfocus() noexcept override;
                void draw() const noexcept override;
                bool isUnder(const Coord& coord) const noexcept override;

            public:
                Button(const Coord& position,
                      const Tag& tag,
                      const std::string& value,
                      const std::function<void()> feedback) noexcept;
        };

        class Field : public UiElement, public Interactable {
            private:
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };

                const unsigned int m_Width;
                std::string m_Value;
                State m_State;

            private:
                void setColorByState() const noexcept;

            public:
                bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept override;
                void handleCursorOver() noexcept override;
                void handleCursorAway() noexcept override;
                void unfocus() noexcept override;
                void draw() const noexcept override;
                bool isUnder(const Coord& coord) const noexcept override;

            public:
                Field(const Coord& position,
                      const Tag& tag,
                      unsigned int width,
                      const std::string& initialValue = "") noexcept;
        };

        class Window {
            private:
                std::vector<Label> m_Labels;
                std::vector<Button> m_Buttons;
                std::vector<Field> m_Fields;

            private:
                // TODO: move outside the class
                std::string toString(Display::WindowType windowType);


            public:
                void unfocus();

                // TODO reorder
                Window& addField(const Field& field) noexcept;
                Window& addLabel(const Label& label) noexcept;
                Window& addButton(const Button& button) noexcept;

                std::optional<std::reference_wrapper<Interactable>>
                        getInteractableUnder(const Coord& coord) noexcept;

                void draw() const noexcept;
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

        // Returns the specified window, initializing one if it is the first access
        Window& populateWindow(WindowType windowType);

        // Either completes completely or not at all
        bool shiftCursor(Coord shift) noexcept ;
        // TODO: Check where the cursor ends up and toggle appropriate Stateful objects
        bool setCursor(const Coord& coord) noexcept;
        void draw() const noexcept;
        void drawBorder() const noexcept;
        void drawCursor() const noexcept;
};


#endif
