#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
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

enum class WindowType : unsigned int {
    Settings = 0,
    Game,
    About,
    Count
};

inline constexpr unsigned int toInt(const WindowType& windowType) noexcept {
    return static_cast<unsigned int>(windowType);
}

inline constexpr WindowType toWindowType(unsigned int index) noexcept {
    return static_cast<WindowType>(index);
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
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };

            protected:
                State m_State;
                Interactable(const State& state) noexcept;

            private:
                bool m_Active;

            public:
                inline bool active() const noexcept { return m_Active; }
                inline void setActive() noexcept { m_Active = true; }
                inline void setPassive() noexcept { m_Active = false; }

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

        struct StateColors {
            public:
                Color idle;
                Color highlighted;
                Color focused;

                StateColors(const Color& idle,
                        const Color& highlighted,
                        const Color& focused) noexcept;

                /* Color get(const Interactable::State& state) const noexcept; */
        };

        struct WindowColors {
            public:
                const StateColors headColors;
                const Color borderColor;

                WindowColors(const StateColors& headColors,
                        const Color& borderColor) noexcept;
        };

        class Button : public UiElement, public Interactable {
            private:
                const Coord m_Dimensions;
                const std::string m_Value;
                std::function<void()> m_Feedback;

                StateColors m_StateColors;

            private:
                static StateColors getDefaultStateColors() noexcept;
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
                      const Coord& dimensions,
                      const Tag& tag,
                      const std::string& value,
                      const std::function<void()> feedback) noexcept;

                Button& setStateColors(const StateColors& stateColors) noexcept;
                /* Button& setIdleColor(const Color& color) noexcept; */
                /* Button& setHighlightedColor(const Color& color) noexcept; */
                /* Button& setFocusedColor(const Color& color) noexcept; */
        };

        class Field : public UiElement, public Interactable {
            private:
                const unsigned int m_Width;
                std::string m_Value;

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

                const Coord m_Dimensions;
                const std::pair<unsigned int, unsigned int> m_HeadRange;
                const Color m_BorderColor;

            public:
                static const unsigned int headBottomY;
                static const unsigned int borderStartY;

            private:
                void drawSelf() const noexcept;
                void drawUi() const noexcept;

            public:
                Window(const Coord& dimensions,
                        const std::pair<unsigned int, unsigned int>& headRange,
                        const Color& borderColor) noexcept;
                void unfocus();

                static WindowColors getColorsForWindow(const WindowType& windowtype) noexcept;


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

        // TODO: come up with a way to make it an std::array
        std::vector<Window> m_Windows;
        WindowType m_ActiveWindowType;
        Coord m_Cursor;

        std::vector<Button> m_WindowHeads;

        inline bool inbounds(const Coord& coord) const noexcept {
            return (0 <= coord.y) && (coord.y <= static_cast<int>(m_Height))
                && (0 <= coord.x) && (coord.x <= static_cast<int>(m_Width));
        }

        /* unsigned int getOrderByWindowType(const WindowType& windowType) const noexcept; */
        /* WindowType getWindowTypeByOrder(unsigned int order) const noexcept; */
        /* Color getWindowColor(const WindowType& windowType) const noexcept; */

        /* std::pair<unsigned int, unsigned int> getWindowHeadRange(unsigned int order) const noexcept; */

        void init() noexcept;
        void cleanup() const noexcept;

    public:
        Display() noexcept;
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
        /* void drawBorder() const noexcept; */
        void drawWindows() const noexcept;
        void drawCursor() const noexcept;
};


std::string toString(WindowType windowType);


#endif
