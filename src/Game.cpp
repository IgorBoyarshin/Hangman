#include "Game.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Game
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Game::Game(unsigned int width, unsigned int height,
        Drawer* drawer, Communicator* communicator)
    : m_Display({width, height, drawer}),
      m_Communicator(communicator),
      m_Terminated(false) {}

Game::~Game() {
    cleanup();
}


void Game::init() {
    m_Display.init();
    initDisplay();
}

void Game::loop() {
    while(!m_Terminated) {
        handleInput();
        m_Display.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    cleanup();
}


void Game::initDisplay() {
    m_Display.populateWindow(WindowType::Settings)
        .addHLine({0,0}, 10, {Color::GREEN, Color::RED}, Tag::createEmpty());




    // m_Display.populateWindow(WindowType::Game)
    //     .addLabel({{5,2}, Tag::createNew(), "Inside Game"})
    //     .addButton({{6,1}, {5, 12}, Tag::createNew(), "Press Me", [this](){
    //             static int row = 10;
    //             if (row >= 13) return; else row++;
    //             m_Display.populateWindow(WindowType::Game)
    //                 .addLabel({{row,3}, Tag::createNew(), "New text " + std::to_string(row - 10)});
    //             }});
    //     // .addField({{9,3}, Display::Tag::createNew(), 8, "abc"});
    //
    // m_Display.populateWindow(WindowType::Settings)
    //     .addLabel({{5,6}, Tag::createNew(), "Inside Settings"})
    //     .addButton({{6,5}, {3, 10}, Tag::createNew(), "Press Me", [this](){
    //             static int row = 10;
    //             if (row >= 13) return; else row++;
    //             m_Display.populateWindow(WindowType::Settings)
    //                 .addLabel({{row,7}, Tag::createNew(), "New text " + std::to_string(row - 10)});
    //             }})
    //     .addField({{9,7}, Tag::createNew(), 8, "abc"});

    m_Display.setActiveWindow(WindowType::Settings);
}

void Game::cleanup() {
    // TODO: need to finish Client/Server??
}

void Game::handleInput() {
    Keyboard::blockOnRead(false);

    auto key = Keyboard::read();
    while (key) {
        processInputKey(*key);
        key = Keyboard::read();
    }
}

void Game::processInputKey(Key key) {
    if (key.is('q')) {
        m_Terminated = true;
        return;
    }

    if (auto interactableUnderCursorOpt =
            m_Display.getInteractableUnderCursor();
            interactableUnderCursorOpt) {
        if (const bool inputHandled = interactableUnderCursorOpt->get()
                    .handleInputKey(key, m_Display.getCursor(),
                        std::bind(&Display::setCursor, &m_Display, std::placeholders::_1));
                inputHandled) {
            return;
        }
    }

    // Input hasn't been handled => needs our attention
    if (const auto direction = key.asDirection()) {
        const Coord shift = [](Direction direction) -> Coord {
            switch (direction) {
                case Direction::DOWN:  return {+1, 0};
                case Direction::UP:    return {-1, 0};
                case Direction::LEFT:  return {0, -1};
                case Direction::RIGHT: return {0, +1};
                default: return {0, 0}; // TODO: error
            }
        }(*direction);

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
