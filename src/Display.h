#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <optional>
#include <functional>
#include <unordered_map>
#include <memory> // shared_ptr
#include <cassert> // TODO: remove once all calls to assert() and replace with Log.assert()
#include "utils.h"
#include "Drawer.h"
#include <ncurses.h>
#include <bitset>

// For sleeping
#include <chrono>
#include <thread>


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Misc
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

std::string toString(WindowType windowType);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Display {
    public:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// UiElement
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class UiElement {
            protected:
                Coord m_Position;
                const Tag m_Tag;
                static Drawer* m_Drawer;

            protected:
                UiElement(const Coord& position, const Tag& tag) noexcept;

            public:
                virtual void draw() const noexcept = 0;
                inline Tag getTag() const noexcept { return m_Tag; }
                inline static void setDrawer(Drawer* drawer) { m_Drawer = drawer; }
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interactable
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class Interactable {
            public:
                enum class State {
                    Idle,
                    Highlighted,
                    Focused
                };
            protected:
                State m_State;
            private:
                bool m_Active;

            protected:
                Interactable(const State& state) noexcept;
            public:
                inline bool active() const noexcept { return m_Active; }
                inline void setActive() noexcept { m_Active = true; }
                inline void setPassive() noexcept { m_Active = false; }

                // Returns whether the input was handled. If it wasn't => may
                // need to handle it in the outter layer
                virtual bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept = 0;
                virtual void handleCursorOver() = 0;
                virtual void handleCursorAway() = 0;

                // Called when the current Window is changed or reset to signal
                // the element that it needs to return to an idle state
                virtual void unfocus() noexcept = 0;
                virtual bool isUnder(const Coord& coord) const noexcept = 0;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Label
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class Label : public UiElement {
            private:
                std::string m_Value;

            public:
                void draw() const noexcept override;
                Label(const Coord& position, const Tag& tag,
                        const std::string& value = "") noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// StateColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        struct StateColors {
            public:
                Color idle;
                Color highlighted;
                Color focused;
            public:
                StateColors(const Color& idle,
                            const Color& highlighted,
                            const Color& focused) noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WindowColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        struct WindowColors {
            public:
                const StateColors headColors;
                const Color borderColor;
            public:
                WindowColors(const StateColors& headColors,
                             const Color& borderColor) noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Button
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
                Button(const Coord& position,
                       const Coord& dimensions,
                       const Tag& tag,
                       const std::string& value,
                       const std::function<void()> feedback);
                bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept override;
                void handleCursorOver() override;
                void handleCursorAway() override;
                void unfocus() noexcept override;
                void draw() const noexcept override;
                bool isUnder(const Coord& coord) const noexcept override;
                Button& setStateColors(const StateColors& stateColors) noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Field
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class Field : public UiElement, public Interactable {
            private:
                const unsigned int m_Width;
                std::string m_Value;
            private:
                void setColorByState() const noexcept;
            public:
                Field(const Coord& position,
                      const Tag& tag,
                      unsigned int width,
                      const std::string& initialValue = "") noexcept;
                bool handleInputSymbol(int c, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept override;
                void handleCursorOver() override;
                void handleCursorAway() override;
                void unfocus() noexcept override;
                void draw() const noexcept override;
                bool isUnder(const Coord& coord) const noexcept override;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Window
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class Window : public UiElement {
            private:
                std::vector<Label> m_Labels;
                std::vector<Button> m_Buttons;
                std::vector<Field> m_Fields;
                const Coord m_Dimensions;
                const std::pair<unsigned int, unsigned int> m_HeadRange;
                const Color m_BorderColor;
            public:
                Window(const Coord& position,
                        const Coord& dimensions,
                        const std::pair<unsigned int, unsigned int>& headRange,
                        const Color& borderColor) noexcept;
                void unfocus();
                static WindowColors getColorsForWindow(const WindowType& windowtype) noexcept;
                Window& addField(const Field& field) noexcept;
                Window& addLabel(const Label& label) noexcept;
                Window& addButton(const Button& button) noexcept;
                std::optional<std::reference_wrapper<Interactable>>
                        getInteractableUnder(const Coord& coord) noexcept;
                void draw() const noexcept override;
            private:
                void drawSelf() const noexcept;
                void drawUi() const noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display itself
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    private:
        unsigned int m_Height; // amount of rows
        unsigned int m_Width;  // amount of columns
        static const unsigned int m_HeadHeight;
        // TODO: come up with a way to make it an std::array
        std::vector<Window> m_Windows;
        WindowType m_ActiveWindowType;
        Coord m_Cursor;
        Drawer* m_Drawer;
        std::vector<Button> m_WindowHeads;
    public:
        Display(unsigned int height, unsigned int width, Drawer* drawer) noexcept;
        virtual ~Display();
        void init() noexcept;
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
    private:
        bool inbounds(const Coord& coord) const noexcept;
        void cleanup() const noexcept;
};


#endif
