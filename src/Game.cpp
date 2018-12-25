#include "Game.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Game
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Game::Game(unsigned int width, unsigned int height,
        Drawer* drawer, Communicator* communicator)
    : m_Display({height, width, drawer}),
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
        handleIncoming();
        m_Display.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    cleanup();
}


void Game::handleIncoming() {
    if (auto receiveOpt = m_Communicator->receive()) {
        const std::string receivedMessage = *receiveOpt;
        Log::info() << "Received:" << receivedMessage << std::endl;

        const Message message(receivedMessage);
        switch (message.type()) {
            case MessageType::WannaPlay:
                handleWannaPlay(*message.asWannaPlay());
                break;
            case MessageType::AcceptedPlay:
                handleAcceptedPlay(*message.asAcceptedPlay());
                break;
            case MessageType::RejectedPlay:
                handleRejectedPlay(*message.asRejectedPlay());
                break;
            case MessageType::PlayNoMore:
                handlePlayNoMore(*message.asPlayNoMore());
                break;
            default:
                Log::error().setClass("Game").setFunc("handleIncoming")
                    << "Unhandled MessageType" << std::endl;
                break;
        }
    }
}


void Game::handleWannaPlay(const MessageWannaPlay& message) noexcept {
    // In case there are previious leftovers
    m_Display.clearScreen();

    // Make visible and insert opponent Nick
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().reveal();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().reveal();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().reveal();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo(
            "Oppotent " + message.initiatorNick() + " wants to play!");

    m_PotentialOpponent = PotentialOpponent(
            message.initiatorNick(),
            message.initiatorAddress(),
            message.initiatorPort(),
            message.initiatorWord());
}

void Game::handleAcceptedPlay(const MessageAcceptedPlay& message) noexcept {

}

void Game::handleRejectedPlay(const MessageRejectedPlay& message) noexcept {
    m_Display.clearScreen();
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get()
        .changeTo(message.rejecteeNick() + " rejected!");
}

void Game::handlePlayNoMore(const MessagePlayNoMore& message) noexcept {

}


void Game::processConnectPress() noexcept {
    // Collect all fields and check that they are valid
    const std::string selfNick = m_Display.getFieldByTag(m_Tags.selfNick)->get().value();
    const std::string selfAddr = m_Communicator->getAddress();
    const unsigned int selfPort = m_Communicator->getPort();
    const std::string oppoAddr = m_Display.getFieldByTag(m_Tags.oppoAddr)->get().value();
    const std::string oppoPortStr = m_Display.getFieldByTag(m_Tags.oppoPort)->get().value();
    const std::string word = m_Display.getFieldByTag(m_Tags.oppoWord)->get().value();
    if (selfNick.empty() || oppoAddr.empty() || oppoPortStr.empty() || word.empty()) {
        // Inform about the error and return
        m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Fill the fields!");
        return;
    }
    const unsigned int oppoPort = static_cast<unsigned int>(std::stoi(oppoPortStr));

    // All data is ready, send the connect request
    const std::string message =
        MessageWannaPlay(selfNick, selfAddr, selfPort, word).asPacket();
    m_Communicator->send(oppoAddr, oppoPort, message);

    // Tell the user that we're waiting
    m_Display.clearScreen();
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Waiting reply...");
}

void Game::processAcceptPress() noexcept {
}

void Game::processRejectPress() noexcept {
    // Hide
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().hide();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo("");

    // Just in case
    m_Display.clearScreen();

    // Notify opponent that we rejected
    const std::string selfNick = m_Display.getFieldByTag(m_Tags.selfNick)->get().value();
    const std::string message =
        MessageRejectedPlay(selfNick).asPacket();
    m_Communicator->send(m_PotentialOpponent.address, m_PotentialOpponent.port, message);
}

void Game::processDisconnectPress() noexcept {
}


void Game::initDisplay() {
    const unsigned int WIDTH = m_Display.getUiWidth();
    /* const unsigned int HEIGHT = m_Display.getUiHeight(); */


    const bool HIDDEN = true;
    m_Display.populateWindow(WindowType::Settings)
        .addLabel({1,1}, Tag::createEmpty(), "YourAddr:")
        .addLabel({1,1+9}, Tag::createEmpty(), m_Communicator->getAddress())
        .addLabel({2,1}, Tag::createEmpty(), "YourPort:")
        .addLabel({2,1+9}, Tag::createEmpty(), std::to_string(m_Communicator->getPort()))
        .addLabel({3,1}, Tag::createEmpty(), "YourNick:")
        .addField({3,1+9}, m_Tags.selfNick, 15, "Igorek")

        .addVLine({0, 27}, 7, {Color::GREEN, Color::CYAN}, Tag::createEmpty())

        .addLabel({1,29}, Tag::createEmpty(), "OppoAddr:")
        .addField({1,29+9}, m_Tags.oppoAddr, 15, "127.0.0.1")
        .addLabel({2,29}, Tag::createEmpty(), "OppoPort:")
        .addField({2,29+9}, m_Tags.oppoPort, 4, "3141")
        .addLabel({3,29}, Tag::createEmpty(), "WordForOppo:")
        .addField({3,29+12}, m_Tags.oppoWord, 15, "unpredictable")
        .addButton({4,29}, {3,11}, m_Tags.connectButton, "Connect",
            std::bind(&Game::processConnectPress, this)
        )
        .addLabel({4+1,29+10+2}, m_Tags.connectionStatus)

        .addHLine({7, 0}, WIDTH, {Color::GREEN, Color::CYAN}, Tag::createEmpty())

        .addLabel({8,1}, m_Tags.oppoWants, "Opponent Masik wants to play!", HIDDEN)
        .addButton({9, 1}, {3, 10}, m_Tags.acceptButton, "Accept",
            std::bind(&Game::processAcceptPress, this), HIDDEN)
        .addButton({9, 1 + 10 + 1}, {3, 10}, m_Tags.rejectButton, "Reject",
            std::bind(&Game::processRejectPress, this), HIDDEN)

        .addHLine({12, 0}, WIDTH, {Color::GREEN, Color::CYAN}, Tag::createEmpty())

        .addButton({13, 1}, {3, 6}, m_Tags.exitButton, "Exit", [](){})
        .addVLine({13, 27}, 3, {Color::GREEN, Color::CYAN}, Tag::createEmpty())
        .addButton({13, 27 + 2}, {3, 12}, m_Tags.disconnectButton, "Disconnect", [](){})
        ;

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
