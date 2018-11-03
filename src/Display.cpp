#include "Display.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Coord
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Coord::Coord() : Coord(0, 0) {}


Display::Coord::Coord(int y, int x) : y(y), x(x) {}


Display::Coord& Display::Coord::operator+=(const Display::Coord& coord) noexcept {
    y += coord.y;
    x += coord.x;
    return *this;
}

Display::Coord Display::Coord::operator+(const Display::Coord& rhs) const noexcept {
    return {y + rhs.y, x + rhs.x};
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Coord
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Color::Color(unsigned int fg, unsigned int bg) noexcept : fg(fg), bg(bg) {};

unsigned int Display::Color::BLACK = 0;
unsigned int Display::Color::RED = 1;
unsigned int Display::Color::GREEN = 2;
unsigned int Display::Color::YELLOW = 3;
unsigned int Display::Color::BLUE = 4;
unsigned int Display::Color::PURPLE = 5;
unsigned int Display::Color::CYAN = 6;
unsigned int Display::Color::WHITE = 7;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::ColorFactory
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned int Display::ColorFactory::m_NextAvailableIndex = 1;
std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>>
    Display::ColorFactory::m_Colors{};

// TODO: Test
void Display::ColorFactory::setColor(const Color& color) noexcept {
    const unsigned int fg = color.fg;
    const unsigned int bg = color.bg;

    if (m_Colors.find(fg) == m_Colors.end() ||
            m_Colors[fg].find(bg) == m_Colors[fg].end()) {
        // If either doesn't exist => no such pair yet => create
        init_pair(m_NextAvailableIndex, fg, bg);
        if (m_Colors.find(fg) == m_Colors.end()) m_Colors[fg] = {};
        m_Colors[fg][bg] = m_NextAvailableIndex++;
    }

    attron(COLOR_PAIR(m_Colors[fg][bg]));
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Tag
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const unsigned int Display::Tag::m_EmptyTag = 0;
unsigned int Display::Tag::m_NextAvailableIndex = Display::Tag::m_EmptyTag + 1;

Display::Tag::Tag(unsigned int value) noexcept : value(value) {}

bool Display::Tag::isEmpty() const noexcept {
    return value == m_EmptyTag;
}

unsigned int Display::Tag::operator()() const noexcept {
    return value;
}

Display::Tag Display::Tag::createEmpty() noexcept {
    return {m_EmptyTag};
}

Display::Tag Display::Tag::createNew() noexcept {
    return {m_NextAvailableIndex++};
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::UiElement
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::UiElement::UiElement(const Coord& position, const Tag& tag) noexcept
    : m_Position(position), m_Tag(tag) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Interactable
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Interactable::Interactable(const State& state) noexcept
    : m_State(state), m_Active(true) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Label
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Label::Label(
        const Display::Coord& position,
        const Tag& tag,
        const std::string& initialValue/* = ""*/) noexcept
        : UiElement(position, tag), m_Value(initialValue) {}

void Display::Label::draw() const noexcept {
    ColorFactory::setColor({Color::WHITE, Color::BLACK});
    move(m_Position.y, m_Position.x);
    addstr(m_Value.c_str());
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::StateColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::StateColors::StateColors(const Color& idle,
                        const Color& highlighted,
                        const Color& focused) noexcept
    : idle(idle), highlighted(highlighted), focused(focused) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Button
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Button::Button(
        const Display::Coord& position,
        const Display::Coord& dimensions,
        const Tag& tag,
        const std::string& value,
        const std::function<void()> feedback) noexcept
        : UiElement(position, tag),
        Interactable(State::Idle),
        m_Dimensions(dimensions),
        m_Value(value),
        m_Feedback(feedback),
        m_StateColors(getDefaultStateColors()) {}

Display::Button& Display::Button::setStateColors(const StateColors& stateColors) noexcept {
    m_StateColors = stateColors;
    return *this;
}

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
            ColorFactory::setColor(m_StateColors.idle);
            break;
        case State::Highlighted:
            ColorFactory::setColor(m_StateColors.highlighted);
            break;
        case State::Focused:
            ColorFactory::setColor(m_StateColors.focused);
            break;
        default:
            // TODO: Log.assert(false)
            ColorFactory::setColor({0,0});
    }
}

bool Display::Button::handleInputSymbol(int c, const Coord& coord,
        const std::function<bool(const Coord&)>& setCursor) noexcept {
    // TODO: assert: isUnder(coord)
    static const unsigned int ENTER = 10;
    if (m_State == State::Highlighted) {
        if (c == ENTER) {
            // TODO: Fix cursor movement while changing state
            m_State = State::Focused;
            draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(140));

            m_State = State::Highlighted;
            draw();

            // Discard any input characters that happened during our sleep
            flushinp();

            // Execute provided beedback function
            m_Feedback();

            return true;
        }
    }

    return false;
}

void Display::Button::handleCursorOver() noexcept {
    if (!active()) return;
    if (m_State == State::Idle) {
        m_State = State::Highlighted;
        draw();
    }
}

void Display::Button::handleCursorAway() noexcept {
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
    setColorByState();

    const Coord textRelativeCoord{
        static_cast<int>(m_Dimensions.y / 2),
        static_cast<int>(m_Dimensions.x - m_Value.size()) / 2
    };

    // Draw upper border
    if (m_Dimensions.y >= 2) {
        move(m_Position.y, m_Position.x);
        addch(ACS_ULCORNER);
        for (int x = 1; x < m_Dimensions.x - 1; x++) addch(ACS_HLINE);
        addch(ACS_URCORNER);
    }
    // Draw void between upper border and text
    for (int y = 1; y < textRelativeCoord.y; y++) {
        move(m_Position.y + y, m_Position.x);
        addch(ACS_VLINE);
        for (int x = 1; x < m_Dimensions.x - 1; x++) addch(' ');
        addch(ACS_VLINE);
    }
    // Draw text
    attron(A_BOLD);
    move(m_Position.y + textRelativeCoord.y, m_Position.x);
    addch(ACS_VLINE);
    for (int x = 1; x < textRelativeCoord.x; x++) addch(' ');
    addstr(m_Value.c_str());
    for (int x = m_Position.x + textRelativeCoord.x + m_Value.size();
            x < m_Position.x + m_Dimensions.x - 1; x++) addch(' ');
    addch(ACS_VLINE);
    attroff(A_BOLD);
    // Draw void between text and bottom border
    for (int y = textRelativeCoord.y + 1; y < m_Dimensions.y - 1; y++) {
        move(m_Position.y + y, m_Position.x);
        addch(ACS_VLINE);
        for (int x = 1; x < m_Dimensions.x - 1; x++) addch(' ');
        addch(ACS_VLINE);
    }
    // Draw bottom border
    if (m_Dimensions.y >= 3) {
        move(m_Position.y + m_Dimensions.y - 1, m_Position.x);
        addch(ACS_LLCORNER);
        for (int x = 1; x < m_Dimensions.x - 1; x++) addch(ACS_HLINE);
        addch(ACS_LRCORNER);
    }

    refresh();
}

// TODO: test
bool Display::Button::isUnder(const Coord& coord) const noexcept {
    return (m_Position.x <= coord.x && coord.x < m_Position.x + m_Dimensions.x) &&
            (m_Position.y <= coord.y && coord.y < m_Position.y + m_Dimensions.y);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Field
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Field::Field(
        const Display::Coord& position,
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
            ColorFactory::setColor({Color::WHITE, Color::BLACK});
            break;
        case State::Highlighted:
            ColorFactory::setColor({Color::BLACK, Color::YELLOW});
            break;
        case State::Focused:
            ColorFactory::setColor({Color::WHITE, Color::BLUE});
            break;
        default:
            // TODO: Log.assert(false)
            ColorFactory::setColor({0,0});
    }
}

bool Display::Field::handleInputSymbol(int c, const Coord& coord,
        const std::function<bool(const Coord&)>& setCursor) noexcept {
    // TODO: assert: isUnder(coord)
    const unsigned int dx = coord.x - m_Position.x;
    const bool cursorInsideValue = dx < m_Value.size();
    static const unsigned int ENTER = 10;

    if (m_State == State::Highlighted) {
        if (c == ENTER) {
            // will change current Display's Cursor, exactly what we want
            setCursor(m_Position + Coord{0,
                    static_cast<int>(cursorInsideValue ? dx : m_Value.size())});

            m_State = State::Focused;
            return true;
        }
    }

    if (m_State == State::Focused) {
        // Discard propagation
        if (c == KEY_DOWN || c == KEY_UP) return true;
        if (c == KEY_LEFT && dx == 0) return true;
        if (c == KEY_RIGHT && dx >= m_Value.size()) return true;

        if (c == ENTER) {
            m_State = State::Highlighted;
            return true;
        }
        if (c == KEY_DC) {
            if (cursorInsideValue) {
                m_Value.erase(m_Value.begin() + dx);
            }
            return true;
        }
        if (isLetter(c) || isNumber(c)) {
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
        // TODO: Investigate: Backspace doesn't work
        /* if (c == KEY_BACKSPACE) { */
        /*     if (0 < dx && dx <= m_Value.size()) { */
        /*         m_Value.erase(m_Value.begin() + dx - 1); */
        /*  */
        /*         // will change current Display's Cursor, exactly what we want */
        /*         setCursor(m_Position + Coord{0, */
        /*                 static_cast<int>(cursorInsideValue ? dx : m_Value.size())}); */
        /*     } */
        /*     return true; */
        /* } */
    }

    return false;
}

void Display::Field::handleCursorOver() noexcept {
    if (m_State == State::Idle) {
        m_State = State::Highlighted;
        draw();
    }
}

void Display::Field::handleCursorAway() noexcept {
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
    setColorByState();

    move(m_Position.y, m_Position.x);
    attron(A_UNDERLINE);
    for (unsigned int x = 0; x < m_Width; x++) {
        const char c = (x < m_Value.size()) ? m_Value[x] : ' ';
        addch(c);
    }
    attroff(A_UNDERLINE);

    refresh();
}

// TODO: test
bool Display::Field::isUnder(const Coord& coord) const noexcept {
    if (coord.y == m_Position.y) {
        return (m_Position.x <= coord.x &&
                coord.x < m_Position.x + static_cast<int>(m_Width));
    }

    return false;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::WindowColors
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::WindowColors::WindowColors(const StateColors& headColors,
                        const Color& borderColor) noexcept :
    headColors(headColors), borderColor(borderColor) {}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Window
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const unsigned int Display::Window::headBottomY = 2;
const unsigned int Display::Window::borderStartY = headBottomY + 1;

Display::Window::Window(
        const Coord& dimensions,
        const std::pair<unsigned int, unsigned int>& headRange,
        const Color& borderColor) noexcept
    : m_Dimensions(dimensions), m_HeadRange(headRange), m_BorderColor(borderColor) {}

void Display::Window::unfocus() {
    // TODO: complete for others
    for (Interactable& interactable : m_Fields) {
        interactable.unfocus();
    }
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

Display::Window& Display::Window::addField(const Field& field) noexcept {
    m_Fields.push_back(field);
    return *this;
}
Display::Window& Display::Window::addLabel(const Label& label) noexcept {
    m_Labels.push_back(label);
    return *this;
}
Display::Window& Display::Window::addButton(const Button& button) noexcept {
    m_Buttons.push_back(button);
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

void Display::Window::drawSelf() const noexcept {
    ColorFactory::setColor(m_BorderColor);

    // Left
    for (int y = 0; y < m_Dimensions.y; y++) {
        move(borderStartY + y, 0);
        addch(' ');
    }
    // Bottom
    move(borderStartY + m_Dimensions.y - 1, 0);
    for (int x = 0; x < m_Dimensions.x; x++) {
        addch(' ');
    }
    // Right
    for (int y = 0; y < m_Dimensions.y; y++) {
        move(borderStartY + y, m_Dimensions.x - 1);
        addch(' ');
    }
    // Top
    move(borderStartY, 0);
    for (int x = 0; x < m_Dimensions.x; x++) {
        addch(' ');
    }
}

void Display::Window::drawUi() const noexcept {
    for (const Field& field : m_Fields) field.draw();
    for (const Label& label : m_Labels) label.draw();
    for (const Button& button : m_Buttons) button.draw();
}

void Display::Window::draw() const noexcept {
    drawSelf();
    drawUi();
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Display() noexcept : m_Height(20), m_Width(60),
    m_ActiveWindowType(WindowType::Game), m_Cursor({0, 0}) {
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
                static_cast<int>(m_Height - Window::borderStartY),
                static_cast<int>(m_Width)
            }, // TODO: move into Game
            windowHeadRange,
            windowColor
        );
        m_WindowHeads.push_back(Button{
            Coord{0, static_cast<int>(windowHeadRange.first)},
            Coord{
                Window::borderStartY,
                static_cast<int>(windowHeadRange.second - windowHeadRange.first + 1)
            },
            Display::Tag::createEmpty(),
            toString(windowType),
            [](){}
        }.setStateColors(headColors));
    }

    setActiveWindow(WindowType::Game);

    init();
}

Display::~Display() {
    cleanup();
}

void Display::init() noexcept {
    initscr();
    noecho();
    start_color();
    keypad(stdscr, TRUE); // enable extended keys
    // getmaxyx(stdscr, m_Height, m_Width);
}


void Display::cleanup() const noexcept {
    endwin();
}

Display::Coord Display::getCursor() const noexcept {
    return m_Cursor;
}

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::getInteractableUnderCursor() noexcept {
    for (Interactable& interactable : m_WindowHeads) {
        if (interactable.isUnder(m_Cursor)) return {{interactable}};
    }
    return m_Windows[toInt(m_ActiveWindowType)].getInteractableUnder(m_Cursor);
}

void Display::setActiveWindow(WindowType windowType) {
    /* if (m_ActiveWindowType == windowType) return; */
    m_Windows[toInt(m_ActiveWindowType)].unfocus();
    m_WindowHeads[toInt(m_ActiveWindowType)].setActive();

    m_ActiveWindowType = windowType;
    m_WindowHeads[toInt(windowType)].setPassive();
    setCursor({0, 0}); // default cursor position in new window
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
        move(m_Cursor.y, m_Cursor.x);
        refresh();
        return true;
    }
    return false;
}

void Display::draw() const noexcept {
    // drawBorder();
    drawWindows();

    // Must come last
    drawCursor();
    refresh();
}

void Display::drawWindows() const noexcept {
    // Draw windows' heads === buttons
    for (const Button& button : m_WindowHeads) button.draw();
    // Draw actual current window
    m_Windows[toInt(m_ActiveWindowType)].draw();

    refresh();
}

/* void Display::drawBorder() const noexcept { */
/*     ColorFactory::setColor({Color::RED, Color::BLACK}); */
/*     border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, */
/*             ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER); */
/* } */
/*  */

void Display::drawCursor() const noexcept {
    move(m_Cursor.y, m_Cursor.x);
    refresh();
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Other
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
