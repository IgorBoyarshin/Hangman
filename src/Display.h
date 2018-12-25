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
#include <cassert> // TODO: remove once all calls to assert() are replaced with Log.assert()
#include "utils.h"
#include "Drawer.h"
#include "Key.h"
#include "Keyboard.h"

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
                bool m_Hidden;

                static Drawer* m_Drawer;

            protected:
                UiElement(const Coord& position, const Tag& tag, bool hidden = false) noexcept;

            public:
                virtual void draw() const noexcept = 0;
                inline Tag getTag() const noexcept { return m_Tag; }
                inline static void setDrawer(Drawer* drawer) { m_Drawer = drawer; }
                inline void hide() noexcept { m_Hidden = true; }
                inline void reveal() noexcept { m_Hidden = false; }
                inline bool hidden() const noexcept { return m_Hidden; }
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
                virtual bool handleInputKey(const Key& key, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept = 0;
                virtual void handleCursorOver() = 0;
                virtual void handleCursorAway() = 0;

                // Called when the current Window is changed or reset to signal
                // the element that it needs to return to an idle state
                virtual void unfocus() noexcept = 0;
                virtual bool isUnder(const Coord& coord) const noexcept = 0;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Spinner
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // class Spinner : public UiElement {
        //     private:
        //         unsigned int m_State;
        //
        //     public:
        //         void draw() const noexcept override;
        //         Spinner(const Coord& position, const Tag& tag) noexcept;
        // };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// VLine
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class VLine : public UiElement {
            private:
                const unsigned int m_Length;
                const Color m_Color;

            public:
                void draw() const noexcept override;
                VLine(const Coord& position, unsigned int length,
                        const Color& color, const Tag& tag) noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// HLine
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class HLine : public UiElement {
            private:
                const unsigned int m_Length;
                const Color m_Color;

            public:
                void draw() const noexcept override;
                HLine(const Coord& position, unsigned int length,
                        const Color& color, const Tag& tag) noexcept;
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
                        const std::string& value = "", bool hidden = false) noexcept;
                void changeTo(const std::string& newValue) noexcept;
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
                       const std::function<void()> feedback,
                       bool hidden = false);
                bool handleInputKey(const Key& key, const Coord& coord,
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
                bool handleInputKey(const Key& key, const Coord& coord,
                        const std::function<bool(const Coord&)>& setCursor) noexcept override;
                void handleCursorOver() override;
                void handleCursorAway() override;
                void unfocus() noexcept override;
                void draw() const noexcept override;
                bool isUnder(const Coord& coord) const noexcept override;
                const std::string& value() const noexcept;
                void changeTo(const std::string& newValue) noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Window
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class Window : public UiElement {
            private:
                std::vector<Label> m_Labels;
                std::vector<Button> m_Buttons;
                std::vector<Field> m_Fields;
                std::vector<VLine> m_VLines;
                std::vector<HLine> m_HLines;
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
                Window& addField(
                        const Coord& position,
                        const Tag tag,
                        unsigned int width,
                        const std::string& initialValue = "") noexcept;
                Window& addLabel(
                        const Coord& position, const Tag tag,
                        const std::string& value = "", bool hidden = false) noexcept;
                Window& addButton(
                        const Coord& position,
                        const Coord& dimensions,
                        const Tag tag,
                        const std::string& value,
                        const std::function<void()> feedback, bool hidden = false) noexcept;
                Window& addVLine(
                        const Coord& position,
                        unsigned int length,
                        const Color& color,
                        const Tag tag) noexcept;
                Window& addHLine(
                        const Coord& position,
                        unsigned int length,
                        const Color& color,
                        const Tag tag) noexcept;
                std::optional<std::reference_wrapper<Interactable>>
                        getInteractableUnder(const Coord& coord) noexcept;
                std::optional<std::reference_wrapper<Label>>
                        getLabelByTag(const Tag& tag) noexcept;
                std::optional<std::reference_wrapper<Button>>
                        getButtonByTag(const Tag& tag) noexcept;
                std::optional<std::reference_wrapper<Field>>
                        getFieldByTag(const Tag& tag) noexcept;
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
        // TODO: come up with a way to make it an std::array
        std::vector<Window> m_Windows;
        WindowType m_ActiveWindowType;
        Coord m_Cursor;
        Drawer* m_Drawer;
        std::vector<Button> m_WindowHeads;
    public:
        static const unsigned int m_HeadHeight;

        Display(unsigned int height, unsigned int width, Drawer* drawer) noexcept;
        virtual ~Display();
        void init() noexcept;
        Coord getCursor() const noexcept;
        std::optional<std::reference_wrapper<Interactable>>
                getInteractableUnderCursor() noexcept;
        std::optional<std::reference_wrapper<Label>>
                getLabelByTag(const Tag& tag) noexcept;
        std::optional<std::reference_wrapper<Button>>
                getButtonByTag(const Tag& tag) noexcept;
        std::optional<std::reference_wrapper<Field>>
                getFieldByTag(const Tag& tag) noexcept;
        void disableWindow(WindowType windowType) noexcept;
        void enableWindow(WindowType windowType) noexcept;
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
        void drawGallows() const noexcept;
        unsigned int getUiWidth() const noexcept;
        unsigned int getUiHeight() const noexcept;
        void clearScreen() const noexcept;
    private:
        bool inbounds(const Coord& coord) const noexcept;
        void cleanup() const noexcept;
};


#endif
