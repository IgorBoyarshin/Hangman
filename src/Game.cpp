#include "Game.h"


Game::Game(Communicator* communicator)
    : m_Display({20, 60, std::make_shared<Renderer>(20, 60)}), m_Communicator(communicator) {}

Game::~Game() {
    cleanup();
}

bool isMovementChar(int c) {
    return (c == 'j') || (c == 'k') || (c == 'h') || (c == 'l') ||
           (c == KEY_DOWN) || (c == KEY_UP) || (c == KEY_LEFT) || (c == KEY_RIGHT);
}

// TODO: TEST
bool isUpperCase(int c) {
    if (isStandartChar(c)) {
        return ('A' <= c) && (c <= 'Z');
    }

    // Otherwise extended char => can't determine
    return false;
}

void Game::init() {
    noecho();
    cbreak(); // no buffering (i.e. no waiting for carriage return)

    initDisplay();
}

void Game::initDisplay() {
    m_Display.populateWindow(WindowType::Game)
        .addLabel({{5,2}, Tag::createNew(), "Inside Game"})
        .addButton({{6,1}, {5, 12}, Tag::createNew(), "Press Me", [this](){
                static int row = 10;
                if (row >= 13) return; else row++;
                m_Display.populateWindow(WindowType::Game)
                    .addLabel({{row,3}, Tag::createNew(), "New text " + std::to_string(row - 10)});
                }});
        // .addField({{9,3}, Display::Tag::createNew(), 8, "abc"});

    m_Display.populateWindow(WindowType::Settings)
        .addLabel({{5,6}, Tag::createNew(), "Inside Settings"})
        .addButton({{6,5}, {3, 10}, Tag::createNew(), "Press Me", [this](){
                static int row = 10;
                if (row >= 13) return; else row++;
                m_Display.populateWindow(WindowType::Settings)
                    .addLabel({{row,7}, Tag::createNew(), "New text " + std::to_string(row - 10)});
                }})
        .addField({{9,7}, Tag::createNew(), 8, "abc"});

    m_Display.setActiveWindow(WindowType::Game);
}

void Game::cleanup() {
    endwin();
    // TODO: need to finish Client/Server??
}

/* void Game::quit() { */
/*     // TODO: Log: quitting by user */
/*     cleanup(); */
/*     exit(0); */
/* } */

void Game::loop() {
    while(true) {
        handleInput();
        m_Display.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void Game::handleInput() {
    nodelay(stdscr, TRUE); // don't block on getch()

    int c = getch();
    while (c != ERR) {
        processInputSymbol(c);
        c = getch();
    }
}

void Game::processInputSymbol(int c) {
    if (c == 'q') {
        cleanup();
        exit(0);
    }

    if (auto interactableUnderCursorOpt =
            m_Display.getInteractableUnderCursor();
            interactableUnderCursorOpt) {
        if (const bool inputHandled = interactableUnderCursorOpt->get()
                    .handleInputSymbol(c, m_Display.getCursor(),
                        std::bind(&Display::setCursor, &m_Display, std::placeholders::_1));
                inputHandled) {
            return;
        }
    }

    // Input hasn't been handled => needs our attention
    if (isMovementChar(c)) {
        const Coord shift = [c]() -> Coord {
            if (c == 'j' || c == KEY_DOWN) {
                return {+1, 0};
            } else if (c == 'k' || c == KEY_UP) {
                return {-1, 0};
            } else if (c == 'h' || c == KEY_LEFT) {
                return {0, -1};
            } else if (c == 'l' || c == KEY_RIGHT) {
                return {0, +1};
            } else {
                // TODO: assert(false)
                return {};
            }
        }();

        auto oldInteractableUnderCursorOpt = m_Display.getInteractableUnderCursor();
        m_Display.shiftCursor(shift);
        auto newInteractableUnderCursorOpt = m_Display.getInteractableUnderCursor();

        // Objects are the same object if both exist and the thing their
        // std::reference_wrapper point to have the same address
        if (const bool sameObject = oldInteractableUnderCursorOpt &&
                                   newInteractableUnderCursorOpt &&
                                   (&(oldInteractableUnderCursorOpt->get()) ==
                                    &(newInteractableUnderCursorOpt)->get());
                !sameObject) {
            if (oldInteractableUnderCursorOpt) {
                oldInteractableUnderCursorOpt->get().handleCursorAway();
            }
            if (newInteractableUnderCursorOpt) {
                newInteractableUnderCursorOpt->get().handleCursorOver();
            }
        }
    }
}

bool Game::connect(const std::string& ip, const std::string port) {
    if (!m_Communicator) return false;
    return m_Communicator->establishConnection(ip, port);
}

bool Game::send(const std::string& message) {
    if (!m_Communicator) return false;
    if (!m_Communicator->connectionEstablished()) return false;
    return m_Communicator->sendMessage(message);
}

std::optional<std::string> Game::receive() {
    if (!m_Communicator) return std::nullopt;
    if (!m_Communicator->connectionEstablished()) return std::nullopt;
    return {m_Communicator->receiveMessage()};
}
