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
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Display::Field
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Display::Field::Field(const Display::Coord& coord,
        unsigned int initialSize, unsigned int maxSize,
        const std::string& initialValue/* = ""*/)
        : m_Coord(coord), m_Size(initialSize),
          m_MaxSize(maxSize), m_Value(initialValue),
          m_State(State::Idle) {}

bool Display::Field::handleInputSymbol(int c, const Coord& coord) noexcept {
    return true;
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
    // TODO
    refresh();
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

void Display::Window::reset() noexcept {
    // TODO: check that the order corresponds to the order of declaration
    // m_Labels.clear();
    // m_Buttons.clear();
    m_Fields.clear();
}

void Display::Window::unfocus() {
    // TODO: complete for others
    for (Interactable& interactable : m_Fields) {
        interactable.unfocus();
    }
}

Display::Window& Display::Window::addLabel(const Coord& coord, std::string text) {
    // TODO
    return *this;
}

Display::Window& Display::Window::addField(
        const Coord& coord,
        unsigned int initialSize, unsigned int maxSize,
        std::string initialValue/* = ""*/) {
    // TODO
    return *this;
}

Display::Window& Display::Window::addButton(const Coord& coord, std::string name,
        std::function<void()> feedback) {
    // TODO
    return *this;
}

std::optional<std::reference_wrapper<Display::Interactable>>
        Display::Window::getInteractableUnder(const Coord& coord) noexcept {
    // TODO
    /* return {std::reference_wrapper<Interactable>{m_Fields[0]}}; */
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
    getmaxyx(stdscr, m_Height, m_Width);
    draw();
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

// Resets the contents of the specified window and returns it
Display::Window& Display::initWindow(WindowType windowType) {
    m_Windows[windowType].reset();
    return m_Windows[windowType];
}

// Returns the specified window
Display::Window& Display::populateWindow(WindowType windowType) {
    return m_Windows[windowType];
}

// Either completes completely or not at all
bool Display::shiftCursor(Coord shift) noexcept {
    return setCursor(shift += m_Cursor); // attention: not the other way around
}

// Check where the cursor ends up and toggle appropriate Stateful objects
bool Display::setCursor(const Coord& coord) noexcept {
    if (inbounds(coord)) {
        m_Cursor = coord;
        move(m_Cursor.y, m_Cursor.x);
        refresh();
        return true;
    }
    return false;
}

void Display::draw() {
    drawBorder();
    refresh();
}


void Display::drawBorder() const noexcept {
    static const chtype HORIZONTAL_BORDER = ACS_HLINE;
    static const chtype VERTICAL_BORDER = ACS_VLINE;
    static const chtype CORNER_LEFT_UP = ACS_ULCORNER;
    static const chtype CORNER_LEFT_DOWN = ACS_LLCORNER;
    static const chtype CORNER_RIGHT_UP = ACS_URCORNER;
    static const chtype CORNER_RIGHT_DOWN = ACS_LRCORNER;

    border(VERTICAL_BORDER, VERTICAL_BORDER, HORIZONTAL_BORDER, HORIZONTAL_BORDER,
            CORNER_LEFT_UP, CORNER_RIGHT_UP, CORNER_LEFT_DOWN, CORNER_RIGHT_DOWN);

    /* move(0, 0); */
    /* addch(CORNER_LEFT_UP); */
    /* for (unsigned int x = 1; x < m_Width - 1; x++) { */
    /*     addch(HORIZONTAL_BORDER); */
    /* } */
    /* addch(CORNER_RIGHT_UP); */
    /*  */
    /* for (unsigned int y = 1; y < m_Height - 1; y++) { */
    /*     mvaddch(y, 0, VERTICAL_BORDER); */
    /* } */
    /*  */
    /* for (unsigned int y = 1; y < m_Height - 1; y++) { */
    /*     mvaddch(y, m_Width - 1, VERTICAL_BORDER); */
    /* } */
    /*  */
    /* move(m_Height - 1, 0); */
    /* addch(CORNER_LEFT_DOWN); */
    /* for (unsigned int x = 1; x < m_Width - 1; x++) { */
    /*     addch(HORIZONTAL_BORDER); */
    /* } */
    /* addch(CORNER_RIGHT_DOWN); */
}
