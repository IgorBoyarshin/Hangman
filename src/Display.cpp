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
// Display::Field
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Field::Field(
        const Display::Coord& position,
        const Tag& tag,
        unsigned int width,
        const std::string& initialValue/* = ""*/) noexcept
        : UiElement(position, tag),
          m_Width(width), m_Value(initialValue),
          m_State(State::Idle) {}

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
            // undraw previous area
            /* move(m_Position.y, m_Position.x); */
            /* ColorFactory::setColor({Color::WHITE, Color::BLACK}); */
            /* for (unsigned int x = 0; x < m_Width; x++) addch(' '); */

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
// Display::Window
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::string Display::Window::toString(Display::WindowType windowType) {
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

/* void Display::Window::reset() noexcept { */
/*     // TODO: check that the order corresponds to the order of declaration */
/*     // m_Labels.clear(); */
/*     // m_Buttons.clear(); */
/*     m_Fields.clear(); */
/* } */

void Display::Window::unfocus() {
    // TODO: complete for others
    for (Interactable& interactable : m_Fields) {
        interactable.unfocus();
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

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::Window::getInteractableUnder(const Coord& coord) noexcept {
    // There are currently 2 types of Interactable: Field and Button
    for (Field& field : m_Fields) {
        if (field.isUnder(coord)) return {{field}};
    }

    return std::nullopt;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Display() : m_Height(0), m_Width(0),
        m_ActiveWindowType(WindowType::Game), m_Cursor({0, 0}) {
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
    getmaxyx(stdscr, m_Height, m_Width);

    // TODO: move into separate function
    m_Windows[WindowType::Settings] = {};
    m_Windows[WindowType::Game] = {};
    m_Windows[WindowType::About] = {};
}


void Display::cleanup() const noexcept {
    endwin();
}

Display::Coord Display::getCursor() const noexcept {
    return m_Cursor;
}

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::getInteractableUnderCursor() noexcept {
    return m_Windows[m_ActiveWindowType].getInteractableUnder(m_Cursor);
}

void Display::setActiveWindow(WindowType windowType) {
    if (m_ActiveWindowType == windowType) return;
    m_Windows[m_ActiveWindowType].unfocus();
    m_ActiveWindowType = windowType;
    setCursor({0, 0}); // default cursor position in new window
}

Display::Window& Display::populateWindow(WindowType windowType) {
    if (m_Windows.find(windowType) == m_Windows.end()) m_Windows[windowType] = {};
    return m_Windows[windowType];
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
    drawBorder();
    m_Windows.at(m_ActiveWindowType).draw();

    // Must come last
    drawCursor();
    refresh();
}

void Display::Window::draw() const noexcept {
    for (const Field& field : m_Fields) field.draw();
    for (const Label& label : m_Labels) label.draw();
}


void Display::drawBorder() const noexcept {
    ColorFactory::setColor({Color::RED, Color::BLACK});
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
}


void Display::drawCursor() const noexcept {
    move(m_Cursor.y, m_Cursor.x);
    refresh();
}
