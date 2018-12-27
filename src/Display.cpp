#include "Display.h"
#include <bitset>


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::UiElement
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Drawer* Display::UiElement::m_Drawer = nullptr;

Display::UiElement::UiElement(const Coord& position, const Tag& tag, bool hidden/* = false*/) noexcept
    : m_Position(position), m_Tag(tag), m_Hidden(hidden) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Interactable
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Interactable::Interactable(const State& state) noexcept
    : m_State(state), m_Active(true) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Spinner
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* Display::Spinner::Spinner( */
/*         const Coord& position, */
/*         const Tag& tag) noexcept */
/*     : UiElement(position, tag) {} */
/*  */
/* void Display::Spinner::draw() const noexcept { */
/*     m_Drawer->setColor({Color::BLUE, Color::BLACK}); */
/*     static const unsigned int SYMBOLS_LENGTH = 4; */
/*     static char symbols[SYMBOLS_LENGTH] = {'|', '/', '-', '|'}; */
/*     m_Drawer->goTo(m_Position.y, m_Position.x); */
/*     m_Drawer->put(symbols[m_State]); */
/*     if (++m_State > SYMBOLS_LENGTH) m_State = 0; */
/* } */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::VLine
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::VLine::VLine(
        const Coord& position,
        unsigned int length,
        const Color& color,
        const Tag& tag) noexcept
    : UiElement(position, tag), m_Length(length), m_Color(color) {}

void Display::VLine::draw() const noexcept {
    if (m_Hidden) return;
    m_Drawer->setColor(m_Color);
    for (unsigned int y = 0; y < m_Length; y++) {
        m_Drawer->goTo(m_Position.y + y, m_Position.x);
        m_Drawer->put(' ');
    }
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::HLine
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::HLine::HLine(
        const Coord& position,
        unsigned int length,
        const Color& color,
        const Tag& tag) noexcept
    : UiElement(position, tag), m_Length(length), m_Color(color) {}

void Display::HLine::draw() const noexcept {
    if (m_Hidden) return;
    m_Drawer->setColor(m_Color);
    m_Drawer->goTo(m_Position.y, m_Position.x);
    for (unsigned int x = 0; x < m_Length; x++) {
        m_Drawer->put(' ');
    }
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Label
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Label::Label(
        const Coord& position,
        const Tag& tag,
        const std::string& initialValue/* = ""*/,
        bool hidden/* = false*/) noexcept
    : UiElement(position, tag, hidden), m_Value(initialValue), m_Color(getDefaultColor()) {}

Color Display::Label::getDefaultColor() noexcept {
    return {Color::WHITE, Color::BLACK};
}

void Display::Label::draw() const noexcept {
    if (m_Hidden) return;
    if (m_Attribute) {
        m_Drawer->setAttribute(*m_Attribute, true);
    }
    m_Drawer->setColor(m_Color);
    m_Drawer->goTo(m_Position.y, m_Position.x);
    m_Drawer->put(m_Value);
    if (m_Attribute) {
        m_Drawer->setAttribute(*m_Attribute, false);
    }
}

Display::Label& Display::Label::changeTo(const std::string& newValue) noexcept {
    m_Value = newValue;
    return *this;
}

Display::Label& Display::Label::setColor(const Color& color) noexcept {
    m_Color = color;
    return *this;
}

Display::Label& Display::Label::setAttribute(
        const Drawer::Attribute attribute, bool on) noexcept {
    if (on) {
        m_Attribute = {attribute};
    } else {
        m_Attribute.reset();
    }
    return *this;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::StateColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::StateColors::StateColors(const Color& idle,
                                  const Color& highlighted,
                                  const Color& focused) noexcept
    : idle(idle), highlighted(highlighted), focused(focused) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::WindowColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::WindowColors::WindowColors(const StateColors& headColors,
                                    const Color& borderColor) noexcept
    : headColors(headColors), borderColor(borderColor) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Button
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Button::Button(
        const Coord& position,
        const Coord& dimensions,
        const Tag& tag,
        const std::string& value,
        const std::function<void()> feedback,
        bool hidden/* = false*/)
    : UiElement(position, tag, hidden),
      Interactable(State::Idle),
      m_Dimensions(dimensions),
      m_Value(value),
      m_Feedback(feedback),
      m_StateColors(getDefaultStateColors()) {}

Display::StateColors Display::Button::getDefaultStateColors() noexcept {
    return {
        {Color::GREEN, Color::BLACK},
        {Color::BLACK, Color::GREEN},
        {Color::BLACK, Color::RED}
    };
}

void Display::Button::setColorByState() const noexcept {
    switch (m_State) {
        case State::Idle:
            m_Drawer->setColor(m_StateColors.idle);
            break;
        case State::Highlighted:
            m_Drawer->setColor(m_StateColors.highlighted);
            break;
        case State::Focused:
            m_Drawer->setColor(m_StateColors.focused);
            break;
        default:
            // TODO: Log.assert(false)
            m_Drawer->setColor({0,0});
    }
}

bool Display::Button::handleInputKey(const Key& key, const Coord& coord,
        const std::function<bool(const Coord&)>& setCursor) noexcept {
    if (m_Hidden) return false;
    if (!active()) return false;
    // TODO: assert: isUnder(coord)
    if (m_State == State::Highlighted) {
        if (key.is(Key::SpecialKey::ENTER)) {
            // TODO: Fix cursor movement while changing state
            m_State = State::Focused;
            draw();
            m_Drawer->update();

            std::this_thread::sleep_for(std::chrono::milliseconds(140));

            // Discard any input characters that happened during our sleep
            Keyboard::discardCharBuffer();

            // Execute provided beedback function
            m_Feedback();

            m_State = State::Highlighted;
            if (!m_Hidden) {
                draw();
                m_Drawer->update();
            }

            return true;
        }
    }

    return false;
}

void Display::Button::handleCursorOver() {
    if (m_Hidden) return;
    if (!active()) return;
    if (m_State == State::Idle) {
        m_State = State::Highlighted;
        draw();
    }
}

void Display::Button::handleCursorAway() {
    if (m_Hidden) return;
    if (m_State == State::Highlighted) {
        m_State = State::Idle;
        draw();
    }
}

void Display::Button::unfocus() noexcept {
    m_State = State::Idle;
    // TODO: mb draw()??
    // TODO: check that the button is OK
}

void Display::Button::draw() const noexcept {
    if (m_Hidden) return;
    setColorByState();

    const Coord textRelativeCoord{
        static_cast<int>(m_Dimensions.y / 2),
        static_cast<int>(m_Dimensions.x - m_Value.size()) / 2
    };

    // Draw upper border
    if (m_Dimensions.y >= 2) {
        m_Drawer->goTo(m_Position.y, m_Position.x);
        m_Drawer->put(Drawer::SpecialChar::ULCORNER);
        for (int x = 1; x < m_Dimensions.x - 1; x++) {
            m_Drawer->put(Drawer::SpecialChar::HLINE);
        }
        m_Drawer->put(Drawer::SpecialChar::URCORNER);
    }
    // Draw void between upper border and text
    for (int y = 1; y < textRelativeCoord.y; y++) {
        m_Drawer->goTo(m_Position.y + y, m_Position.x);
        m_Drawer->put(Drawer::SpecialChar::VLINE);
        for (int x = 1; x < m_Dimensions.x - 1; x++) m_Drawer->put(' ');
        m_Drawer->put(Drawer::SpecialChar::VLINE);
    }
    // Draw text
    m_Drawer->setAttribute(Drawer::Attribute::BOLD, true);
    m_Drawer->goTo(m_Position.y + textRelativeCoord.y, m_Position.x);
    m_Drawer->put(Drawer::SpecialChar::VLINE);

    for (int x = 1; x < textRelativeCoord.x; x++) m_Drawer->put(' ');
    m_Drawer->put(m_Value);
    for (int x = m_Position.x + textRelativeCoord.x + m_Value.size();
            x < m_Position.x + m_Dimensions.x - 1; x++) m_Drawer->put(' ');
    m_Drawer->put(Drawer::SpecialChar::VLINE);
    m_Drawer->setAttribute(Drawer::Attribute::BOLD, false);
    // Draw void between text and bottom border
    for (int y = textRelativeCoord.y + 1; y < m_Dimensions.y - 1; y++) {
        m_Drawer->goTo(m_Position.y + y, m_Position.x);
        m_Drawer->put(Drawer::SpecialChar::VLINE);
        for (int x = 1; x < m_Dimensions.x - 1; x++) m_Drawer->put(' ');
        m_Drawer->put(Drawer::SpecialChar::VLINE);
    }
    // Draw bottom border
    if (m_Dimensions.y >= 3) {
        m_Drawer->goTo(m_Position.y + m_Dimensions.y - 1, m_Position.x);
        m_Drawer->put(Drawer::SpecialChar::LLCORNER);
        for (int x = 1; x < m_Dimensions.x - 1; x++) m_Drawer->put(Drawer::SpecialChar::HLINE);
        m_Drawer->put(Drawer::SpecialChar::LRCORNER);
    }

    // m_Drawer->update();
}

bool Display::Button::isUnder(const Coord& coord) const noexcept {
    if (m_Hidden) return false;
    return (m_Position.x <= coord.x && coord.x < m_Position.x + m_Dimensions.x) &&
            (m_Position.y <= coord.y && coord.y < m_Position.y + m_Dimensions.y);
}

Display::Button& Display::Button::setStateColors(const StateColors& stateColors) noexcept {
    m_StateColors = stateColors;
    return *this;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Field
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Field::Field(
        const Coord& position,
        const Tag& tag,
        unsigned int width,
        const std::string& initialValue/* = ""*/) noexcept
    : UiElement(position, tag),
      Interactable(State::Idle),
      m_Width(width),
      m_Value(initialValue) {}

void Display::Field::setColorByState() const noexcept {
    switch (m_State) {
        case State::Idle:
            m_Drawer->setColor({Color::WHITE, Color::BLACK});
            break;
        case State::Highlighted:
            m_Drawer->setColor({Color::BLACK, Color::YELLOW});
            break;
        case State::Focused:
            m_Drawer->setColor({Color::WHITE, Color::BLUE});
            break;
        default:
            // TODO: Log.assert(false)
            m_Drawer->setColor({0,0});
    }
}

bool Display::Field::handleInputKey(const Key& key, const Coord& coord,
        const std::function<bool(const Coord&)>& setCursor) noexcept {
    if (m_Hidden) return false;
    if (!active()) return false;
    // TODO: assert: isUnder(coord)
    const unsigned int dx = coord.x - m_Position.x;
    const bool cursorInsideValue = dx < m_Value.size();

    if (m_State == State::Highlighted) {
        if (key.is(Key::SpecialKey::ENTER)) {
            // will change current Display's Cursor, exactly what we want
            setCursor(m_Position + Coord{0,
                    static_cast<int>(cursorInsideValue ? dx : m_Value.size())});

            m_State = State::Focused;
            return true;
        }
    }

    if (m_State == State::Focused) {
        // Discard propagation
        if (key.is(Key::SpecialKey::DOWN) || key.is(Key::SpecialKey::UP)) return true;
        if (key.is(Key::SpecialKey::LEFT) && dx == 0) return true;
        if (key.is(Key::SpecialKey::RIGHT) && dx >= m_Value.size()) return true;

        if (key.is(Key::SpecialKey::ENTER)) {
            m_State = State::Highlighted;
            return true;
        }
        if (key.is(Key::SpecialKey::DELETE)) {
            if (cursorInsideValue) {
                m_Value.erase(m_Value.begin() + dx);
            }
            return true;
        }
        if (key.isLetter() || key.isNumber()) {
            const char c = *key.getRegular();
            if (m_Value.size() < m_Width - 1) {
                m_Value.insert(m_Value.begin() + dx, c);
                setCursor(m_Position + Coord{0, static_cast<int>(dx + 1)});
            } else if (m_Value.size() == m_Width - 1) {
                m_Value.insert(m_Value.begin() + dx, c);
                if (dx < m_Value.size() - 1) {
                    setCursor(m_Position + Coord{0, static_cast<int>(dx + 1)});
                }
            }
            return true;
        }

        if (key.is(Key::SpecialKey::BACKSPACE)) {
            if (0 < dx && dx <= m_Value.size()) {
                m_Value.erase(m_Value.begin() + dx - 1); // erase one before current
                setCursor(m_Position + Coord{0, static_cast<int>(dx - 1)});
            }
            return true;
        }
    }

    return false;
}

void Display::Field::handleCursorOver() {
    if (m_Hidden) return;
    if (m_State == State::Idle) {
        m_State = State::Highlighted;
        draw();
    }
}

void Display::Field::handleCursorAway() {
    if (m_Hidden) return;
    if (m_State == State::Highlighted) {
        m_State = State::Idle;
        draw();
    }
}

void Display::Field::unfocus() noexcept {
    m_State = State::Idle;
    // TODO: check that the field is OK
}

void Display::Field::draw() const noexcept {
    if (m_Hidden) return;
    setColorByState();

    m_Drawer->goTo(m_Position.y, m_Position.x);
    m_Drawer->setAttribute(Drawer::Attribute::UNDERLINE, true);
    for (unsigned int x = 0; x < m_Width; x++) {
        const char c = (x < m_Value.size()) ? m_Value[x] : ' ';
        m_Drawer->put(c);
    }
    m_Drawer->setAttribute(Drawer::Attribute::UNDERLINE, false);

    // m_Drawer->update();
}

bool Display::Field::isUnder(const Coord& coord) const noexcept {
    if (m_Hidden) return false;
    if (coord.y == m_Position.y) {
        return (m_Position.x <= coord.x &&
                coord.x < m_Position.x + static_cast<int>(m_Width));
    }

    return false;
}

const std::string& Display::Field::value() const noexcept {
    return m_Value;
}

Display::Field& Display::Field::changeTo(const std::string& newValue) noexcept {
    m_Value = newValue;
    return *this;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Window
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Window::Window(
        const Coord& position,
        const Coord& dimensions,
        const std::pair<unsigned int, unsigned int>& headRange,
        const Color& borderColor) noexcept
    : UiElement(position, Tag::createEmpty()),
      m_Dimensions(dimensions),
      m_HeadRange(headRange),
      m_BorderColor(borderColor) {}

void Display::Window::unfocus() {
    // TODO: complete for others
    for (Interactable& interactable : m_Fields) interactable.unfocus();
    for (Interactable& interactable : m_Buttons) interactable.unfocus();
}

Display::WindowColors Display::Window::getColorsForWindow(const WindowType& windowtype) noexcept {
    switch (windowtype) {
        case WindowType::Settings:
            return {
                {
                    {Color::BLACK, Color::CYAN},
                    {Color::CYAN, Color::BLACK},
                    {Color::RED, Color::CYAN}
                },
                {Color::BLACK, Color::CYAN}
            };
        case WindowType::Game:
            return {
                {
                    {Color::BLACK, Color::YELLOW},
                    {Color::YELLOW, Color::BLACK},
                    {Color::RED, Color::YELLOW}
                },
                {Color::BLACK, Color::YELLOW}
            };
        case WindowType::About:
            return {
                {
                    {Color::BLACK, Color::PURPLE},
                    {Color::PURPLE, Color::BLACK},
                    {Color::RED, Color::PURPLE}
                },
                {Color::BLACK, Color::PURPLE}
            };
        default:
            // TODO: assert(false)
            return {
                {
                    {Color::BLACK, Color::CYAN},
                    {Color::BLUE, Color::CYAN},
                    {Color::RED, Color::CYAN}
                },
                {Color::PURPLE, Color::BLACK}
            };
    }
}

Display::Window& Display::Window::addField(
        const Coord& position,
        const Tag tag,
        unsigned int width,
        const std::string& initialValue/* = ""*/) noexcept {
    const Coord newPosition = topShift + position;
    m_Fields.push_back({newPosition, tag, width, initialValue});
    return *this;
}
Display::Window& Display::Window::addLabel(
        const Coord& position, const Tag tag,
        const std::string& value/* = ""*/,
        bool hidden/* = false*/) noexcept {
    const Coord newPosition = topShift + position;
    m_Labels.push_back({newPosition, tag, value, hidden});
    return *this;
}
Display::Window& Display::Window::addButton(
        const Coord& position,
        const Coord& dimensions,
        const Tag tag,
        const std::string& value,
        const std::function<void()> feedback,
        bool hidden/* = false*/) noexcept {
    const Coord newPosition = topShift + position;
    m_Buttons.push_back({newPosition, dimensions, tag, value, feedback, hidden});
    return *this;
}
Display::Window& Display::Window::addVLine(
        const Coord& position,
        unsigned int length,
        const Color& color,
        const Tag tag) noexcept {
    const Coord newPosition = topShift + position;
    m_VLines.push_back({newPosition, length, color, tag});
    return *this;
}
Display::Window& Display::Window::addHLine(
        const Coord& position,
        unsigned int length,
        const Color& color,
        const Tag tag) noexcept {
    const Coord newPosition = topShift + position;
    m_HLines.push_back({newPosition, length, color, tag});
    return *this;
}

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::Window::getInteractableUnder(const Coord& coord) noexcept {
    // There are currently 2 types of Interactable: Field and Button
    for (Interactable& interactable : m_Fields) {
        if (interactable.isUnder(coord)) return {{interactable}};
    }
    for (Interactable& interactable : m_Buttons) {
        if (interactable.isUnder(coord)) return {{interactable}};
    }

    return std::nullopt;
}

std::optional<std::reference_wrapper<Display::Label>>
        Display::Window::getLabelByTag(const Tag& tag) noexcept {
    for (Label& label : m_Labels) {
        if (label.getTag().is(tag)) return {{label}};
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<Display::Button>>
        Display::Window::getButtonByTag(const Tag& tag) noexcept {
    for (Button& button : m_Buttons) {
        if (button.getTag().is(tag)) return {{button}};
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<Display::Field>>
        Display::Window::getFieldByTag(const Tag& tag) noexcept {
    for (Field& field : m_Fields) {
        if (field.getTag().is(tag)) return {{field}};
    }
    return std::nullopt;
}

void Display::Window::drawSelf() const noexcept {
    m_Drawer->setColor(m_BorderColor);

    // Left
    for (int y = 0; y < m_Dimensions.y; y++) {
        m_Drawer->goTo(m_Position.y + y, 0);
        m_Drawer->put(' ');
    }
    // Bottom
    m_Drawer->goTo(m_Position.y + m_Dimensions.y - 1, 0);
    for (int x = 0; x < m_Dimensions.x; x++) {
        m_Drawer->put(' ');
    }
    // Right
    for (int y = 0; y < m_Dimensions.y; y++) {
        m_Drawer->goTo(m_Position.y + y, m_Dimensions.x - 1);
        m_Drawer->put(' ');
    }
    // Top
    m_Drawer->goTo(m_Position.y, 0);
    for (int x = 0; x < m_Dimensions.x; x++) {
        m_Drawer->put(' ');
    }
}

void Display::Window::drawUi() const noexcept {
    for (const Field& field : m_Fields) field.draw();
    for (const Label& label : m_Labels) label.draw();
    for (const Button& button : m_Buttons) button.draw();
    for (const VLine& hline : m_VLines) hline.draw();
    for (const HLine& hline : m_HLines) hline.draw();
}

void Display::Window::draw() const noexcept {
    drawSelf();
    drawUi();
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const unsigned int Display::m_HeadHeight = 3;
const Coord Display::topShift = Coord{Display::m_HeadHeight + 1, 1};
const unsigned int Display::gallowsSteps = 10;


Display::Display(unsigned int height, unsigned int width, Drawer* drawer) noexcept
        : m_Height(height), m_Width(width),
          m_ActiveWindowType(WindowType::Game),
          m_Cursor({m_HeadHeight + 1, 1}),
          m_Drawer(drawer) {
    UiElement::setDrawer(drawer);

    const auto getWindowHeadRange = [width = m_Width](unsigned int order) noexcept {
        const unsigned int windowsAmount = toInt(WindowType::Count);
        const unsigned int windowWidth = (width / windowsAmount);
        const bool lastWindow = (order == windowsAmount - 1);
        return std::pair<unsigned int, unsigned int>{
            order * windowWidth,
            lastWindow ? (width - 1) : ((order + 1) * windowWidth - 1)
        };
    };

    for (unsigned int i = 0; i < toInt(WindowType::Count); i++) {
        const auto windowHeadRange = getWindowHeadRange(i);
        const WindowType windowType = toWindowType(i);
        const auto [headColors, windowColor] = Window::getColorsForWindow(windowType);

        m_Windows.emplace_back(
            Coord{
                m_HeadHeight,
                0
            },
            Coord{
                static_cast<int>(m_Height - m_HeadHeight),
                static_cast<int>(m_Width)
            }, // TODO: move into Game
            windowHeadRange,
            windowColor
        );
        m_WindowHeads.push_back(Button{
            Coord{0, static_cast<int>(windowHeadRange.first)},
            Coord{
                m_HeadHeight,
                static_cast<int>(windowHeadRange.second - windowHeadRange.first + 1)
            },
            Tag::createEmpty(),
            toString(windowType),
            [windowType, this](){
                setActiveWindow(windowType);
            }
        }.setStateColors(headColors));
    }
}

Display::~Display() {
    cleanup();
}

void Display::init() noexcept {
    m_Drawer->init();
    m_Drawer->enableColors();
    Keyboard::echo(false);
    Keyboard::waitCarriageReturn(false);
    Keyboard::enableSpecialKeys();

    setActiveWindow(WindowType::Game);
}

Coord Display::getCursor() const noexcept {
    return m_Cursor;
}

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::getInteractableUnderCursor() noexcept {
    for (Interactable& interactable : m_WindowHeads) {
        if (interactable.isUnder(m_Cursor)) return {{interactable}};
    }
    return m_Windows[toInt(m_ActiveWindowType)].getInteractableUnder(m_Cursor);
}

std::optional<std::reference_wrapper<Display::Label>>
        Display::getLabelByTag(const Tag& tag) noexcept {
    for (Window& window : m_Windows) {
        if (auto resultOpt = window.getLabelByTag(tag)) return resultOpt;
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<Display::Button>>
        Display::getButtonByTag(const Tag& tag) noexcept {
    for (Window& window : m_Windows) {
        if (auto resultOpt = window.getButtonByTag(tag)) return resultOpt;
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<Display::Field>>
        Display::getFieldByTag(const Tag& tag) noexcept {
    for (Window& window : m_Windows) {
        if (auto resultOpt = window.getFieldByTag(tag)) return resultOpt;
    }
    return std::nullopt;
}


void Display::setActiveWindow(WindowType windowType) {
    m_Windows[toInt(m_ActiveWindowType)].unfocus();
    m_WindowHeads[toInt(windowType)].unfocus();
    m_WindowHeads[toInt(m_ActiveWindowType)].setActive();

    m_ActiveWindowType = windowType;
    m_WindowHeads[toInt(m_ActiveWindowType)].setPassive();
    /* setCursor({0, 0}); // default cursor position in new window */
    // setCursor({Window::borderStartY + 1, 1});
    m_Drawer->clearScreen();
}

WindowType Display::getActiveWindow() const noexcept {
    return m_ActiveWindowType;
}

void Display::disableWindow(WindowType windowType) noexcept {
    m_WindowHeads[toInt(windowType)].setPassive();
}

void Display::enableWindow(WindowType windowType) noexcept {
    m_WindowHeads[toInt(windowType)].setActive();
}

Display::Window& Display::populateWindow(WindowType windowType) {
    return m_Windows[toInt(windowType)];
}

bool Display::shiftCursor(Coord shift) noexcept {
    return setCursor(shift += m_Cursor); // attention: not the other way around
}

bool Display::setCursor(const Coord& coord) noexcept {
    if (inbounds(coord)) {
        m_Cursor = coord;
        m_Drawer->goTo(m_Cursor.y, m_Cursor.x);
        // m_Drawer->update();
        return true;
    }
    return false;
}

void Display::draw() const noexcept {
    // drawBorder();
    drawWindows();

    // Must come last
    drawCursor();

    // Update the changes on screen
    m_Drawer->update();
}

void Display::drawWindows() const noexcept {
    // Draw windows' heads === buttons
    for (const Button& button : m_WindowHeads) button.draw();
    // Draw actual current window
    m_Windows[toInt(m_ActiveWindowType)].draw();
}

void Display::drawCursor() const noexcept {
    m_Drawer->goTo(m_Cursor.y, m_Cursor.x);
    // m_Drawer->update();
}

void Display::drawGallows(Coord coord, unsigned int mistakes) const noexcept {
    coord += topShift;
    m_Drawer->setColor({Color::YELLOW, Color::BLACK});
    m_Drawer->setAttribute(Drawer::Attribute::BOLD, true);
    // m_Drawer->goTo(5, 3);
    // m_Drawer->put('P');
    // m_Drawer->put('l');
    // m_Drawer->put('a');
    // m_Drawer->put('y');
    // m_Drawer->put('e');
    // m_Drawer->put('r');
    // m_Drawer->put('N');
    // m_Drawer->put('i');
    // m_Drawer->put('c');
    // m_Drawer->put('k');
    // m_Drawer->put(':');

    // drawing the gallow depending on the amount of mistakes
    switch (mistakes) {
        case 10: // 10 mistakes
            m_Drawer->goTo(3 + coord.y, 11 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::URCORNER);
            m_Drawer->goTo(4 + coord.y, 11 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::LLCORNER);
            [[fallthrough]];
        case 9: // 9 mistakes
            m_Drawer->goTo(3 + coord.y, 9 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::ULCORNER);
            m_Drawer->goTo(4 + coord.y, 9 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::LRCORNER);
            [[fallthrough]];
        case 8: // 8 mistakes
            m_Drawer->goTo(2 + coord.y, 11 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::URCORNER);
            [[fallthrough]];
        case 7: // 7 mistakes
            m_Drawer->goTo(2 + coord.y, 9 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::ULCORNER);
            [[fallthrough]];
        case 6: // 6 mistakes
            m_Drawer->goTo(2 + coord.y, 10 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::PLUS);
            m_Drawer->goTo(3 + coord.y, 10 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::BTEE);
            [[fallthrough]];
        case 5: // 5 mistakes
            m_Drawer->goTo(1 + coord.y, 10 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::CKBOARD);
            [[fallthrough]];
        case 4: // 4 mistakes
            m_Drawer->goTo(0 + coord.y, 10 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::URCORNER);
            [[fallthrough]];
        case 3: // 3 mistakes
            m_Drawer->goTo(0 + coord.y, 2 + coord.x);
            m_Drawer->put(Drawer::SpecialChar::ULCORNER);
            for (unsigned int h = 0; h < 7; h++) {
                m_Drawer->put(Drawer::SpecialChar::HLINE);
            }
            [[fallthrough]];
        case 2: // 2 mistakes
            for (unsigned int v = 0; v < 4; v++) {
                m_Drawer->goTo(v + 1 + coord.y, 2 + coord.x);
                m_Drawer->put(Drawer::SpecialChar::VLINE);
            }
            [[fallthrough]];
        case 1: // 1 mistake
            m_Drawer->goTo(5 + coord.y, 0 + coord.x);
            for (unsigned int h = 1; h < 6; h++) {
                if (h % 3 == 0) {
                    m_Drawer->put(Drawer::SpecialChar::BTEE);
                    continue;
                }
                m_Drawer->put(Drawer::SpecialChar::HLINE);
            }
        default:
            break;
    }
}

void Display::drawWord(
        unsigned int yLevel, const std::string& word,
        const std::vector<bool> revealed, bool endgame/* = false*/) const noexcept {
    // coord += topShift;
    yLevel += topShift.y;
    const unsigned int xCenter = m_Width / 2;
    /* const unsigned int half = (word.size() / 2 + 1) * 2; */
    // Each letter has 1 space after it
    m_Drawer->setColor({Color::YELLOW, Color::BLACK});
    m_Drawer->goTo(yLevel, xCenter - word.size());
    for (unsigned int i = 0; i < word.size(); i++) {
        if (revealed[i]) {
            m_Drawer->put(word[i]);
        } else { // not revealed
            if (endgame) {
                m_Drawer->setColor({Color::RED, Color::BLACK});
                m_Drawer->put(word[i]);
                m_Drawer->setColor({Color::YELLOW, Color::BLACK});
            } else {
                m_Drawer->put('_');
            }
        }
        m_Drawer->put(' '); // space between letters
    }
}

unsigned int Display::getUiWidth() const noexcept {
    return m_Width - 2;
}

unsigned int Display::getUiHeight() const noexcept {
    return m_Height - m_HeadHeight - 2;
}

void Display::clearScreen() const noexcept {
    m_Drawer->clearScreen();
}

bool Display::inbounds(const Coord& coord) const noexcept {
    return (0 <= coord.y) && (coord.y < static_cast<int>(m_Height))
        && (0 <= coord.x) && (coord.x < static_cast<int>(m_Width));
}

void Display::cleanup() const noexcept {
    // endwin();
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Misc
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::string toString(WindowType windowType) {
    switch (windowType) {
        case WindowType::Settings:
            return "Settings";
        case WindowType::Game:
            return "Game";
        case WindowType::About:
            return "About";
        default:
            // TODO: Log.asert(false);
            // TODO: maybe throw??
            return "";
    }
}
