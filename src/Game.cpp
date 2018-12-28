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
    Keyboard::blockOnRead(false);
}

void Game::loop() {
    while(!m_Terminated) {
        handleInput();
        handleIncoming();
        m_Display.draw();
        if (m_GameContext && m_Display.getActiveWindow() == WindowType::Game) { // if a game is in progress now
            static const Coord gallowsCoord = {1, 22};
            m_Display.drawGallows(gallowsCoord, m_GameContext->mistakesMade);
            m_Display.drawWord(7, m_GameContext->word,
                    m_GameContext->revealed, m_GameContext->ended);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}


void Game::handleIncoming() {
    if (auto receiveOpt = m_Communicator->receive()) {
        const std::string receivedMessage = *receiveOpt;
        /* Log::info() << "Received:" << receivedMessage << std::endl; */

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
            case MessageType::TryLetter:
                handleTryLetter(*message.asTryLetter());
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

    m_InquiringOpponent = InquiringOpponent(
            message.initiatorNick(),
            message.initiatorAddress(),
            message.initiatorPort(),
            message.initiatorWord());
}

void Game::handleAcceptedPlay(const MessageAcceptedPlay& message) noexcept {
    // Hide
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().hide();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo("");
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("");
    // Just in case
    m_Display.clearScreen();

    // Start new game
    m_Display.enableWindow(WindowType::Game);
    m_Display.setActiveWindow(WindowType::Game);
    const std::string selfNick = m_Display.getFieldByTag(m_Tags.selfNick)->get().value();
    const std::string opponentNick = message.accepteeNick();
    const bool HANGING_SELF = false;
    // If we received this message => we've pressed (and sent) Connect button =>
    // => we have m_DesiredOpponent set => can use its field word
    setupNewGame(HANGING_SELF, selfNick, opponentNick, m_DesiredOpponent->word);
}

void Game::handleRejectedPlay(const MessageRejectedPlay& message) noexcept {
    m_Display.clearScreen();
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get()
        .changeTo(message.rejecteeNick() + " rejected!");
}

void Game::handleTryLetter(const MessageTryLetter& message) noexcept {
    if (!m_GameContext) {
        Log::error().setClass("Game").setFunc("handleTryLetter")
            << "Called outside of game context" << std::endl;
        return;
    }

    const char letter = message.letter();
    tryLetter(letter);
    checkEndgame();
}

void Game::handlePlayNoMore(const MessagePlayNoMore& message) noexcept {
    // Reset current query for reply
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().hide();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo("");

    // Just in case
    m_Display.clearScreen();

    // End current game
    m_GameContext.reset();
    m_Display.setActiveWindow(WindowType::Settings);
    m_Display.disableWindow(WindowType::Game);
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Opponent quit!");

    m_GameContext.reset();
    m_DesiredOpponent.reset();
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

    // Store the DesiredOpponent
    m_DesiredOpponent = {DesiredOpponent{oppoAddr, oppoPort, word}};

    // All data is ready, send the connect request
    const std::string message =
        MessageWannaPlay(selfNick, selfAddr, selfPort, word).asPacket();
    m_Communicator->sendAsync(oppoAddr, oppoPort, message);

    // Tell the user that we're waiting
    m_Display.clearScreen();
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Waiting reply...");
}

void Game::processAcceptPress() noexcept {
    const std::string selfNick = m_Display.getFieldByTag(m_Tags.selfNick)->get().value();
    if (selfNick.empty()) {
        m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Need your nick!");
        return;
    }

    // Hide
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().hide();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo("");
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("");
    // Just in case
    m_Display.clearScreen();

    // Notify opponent that we've accepted
    const std::string message = MessageAcceptedPlay(selfNick).asPacket();
    // There is sure to be valid data in m_InquiringOpponent because Accept button
    // wouldn't've been pressable at all otherwise
    // (m_InquiringOpponent is filled upon WannaPlay request)
    m_Communicator->sendAsync(m_InquiringOpponent.address, m_InquiringOpponent.port, message);

    // Need this field filled for various purposes. The meaning is "CurrentOpponent" here
    m_DesiredOpponent = {DesiredOpponent{
        m_InquiringOpponent.address,
        m_InquiringOpponent.port,
        m_InquiringOpponent.word
    }};

    // Start new game
    m_Display.enableWindow(WindowType::Game);
    m_Display.setActiveWindow(WindowType::Game);
    const bool HANGING_SELF = true;
    const std::string opponentNick = m_InquiringOpponent.nick;
    setupNewGame(HANGING_SELF, selfNick, opponentNick, m_DesiredOpponent->word);
}

void Game::processRejectPress() noexcept {
    const std::string selfNick = m_Display.getFieldByTag(m_Tags.selfNick)->get().value();
    if (selfNick.empty()) {
        m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("Need your nick!");
        return;
    }

    // Hide
    m_Display.getButtonByTag(m_Tags.acceptButton)->get().hide();
    m_Display.getButtonByTag(m_Tags.rejectButton)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().hide();
    m_Display.getLabelByTag(m_Tags.oppoWants)->get().changeTo("");
    // Just in case
    m_Display.clearScreen();

    // Notify opponent that we've rejected
    const std::string message = MessageRejectedPlay(selfNick).asPacket();
    // There is sure to be valid data in m_InquiringOpponent because Reject button
    // wouldn't've been pressable at all otherwise
    // (m_InquiringOpponent is filled upon WannaPlay request)
    m_Communicator->sendAsync(m_InquiringOpponent.address, m_InquiringOpponent.port, message);
}

void Game::processDisconnectPress() noexcept {
    // Clear the status bar
    m_Display.getLabelByTag(m_Tags.connectionStatus)->get().changeTo("");
    // Just in case
    m_Display.clearScreen();

    // Notify and reset the opponent if such exists
    if (m_DesiredOpponent) {
        const std::string message = MessagePlayNoMore().asPacket();
        m_Communicator->sendAsync(m_DesiredOpponent->address, m_DesiredOpponent->port, message);
        m_DesiredOpponent.reset();
    }

    m_GameContext.reset();
    m_Display.disableWindow(WindowType::Game);
}

void Game::processExitPress() noexcept {
    // Notify and reset the opponent if such exists
    if (m_DesiredOpponent) {
        const std::string message = MessagePlayNoMore().asPacket();
        m_Communicator->sendAsync(m_DesiredOpponent->address, m_DesiredOpponent->port, message);
        m_DesiredOpponent.reset();
    }

    // Terminate the program
    m_Terminated = true;
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
        // TODO: remove ternary
        .addField({3,1+9}, m_Tags.selfNick, 15, m_Communicator->getPort() == 3141 ? "TomCruise" : "Tortoise")

        .addVLine({0, 27}, 7, {Color::GREEN, Color::CYAN}, Tag::createEmpty())

        .addLabel({1,29}, Tag::createEmpty(), "OppoAddr:")
        .addField({1,29+9}, m_Tags.oppoAddr, 15, "127.0.0.1")
        .addLabel({2,29}, Tag::createEmpty(), "OppoPort:")
        // TODO: remove ternary
        .addField({2,29+9}, m_Tags.oppoPort, 4, m_Communicator->getPort() == 3141 ? "3142" : "3141")
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

        .addButton({13, 1}, {3, 6}, m_Tags.exitButton, "Exit",
            std::bind(&Game::processExitPress, this)
        )
        .addVLine({13, 27}, 3, {Color::GREEN, Color::CYAN}, Tag::createEmpty())
        .addButton({13, 27 + 2}, {3, 12}, m_Tags.disconnectButton, "Disconnect",
            std::bind(&Game::processDisconnectPress, this)
        )
        ;

    m_Display.populateWindow(WindowType::About)
        .addLabel({1, 1}, Tag::createEmpty(),
                "Welcome to the Hangman,")
        .addLabel({2, 15}, Tag::createEmpty(),
                "a game where you get to hang your friends!")
        .addLabel({4, 1}, Tag::createEmpty(),
                "Use either hjkl or arrow keys to navigate.")
        .addLabel({5, 1}, Tag::createEmpty(),
                "Press Enter to interact with a button.")
        .addLabel({6, 1}, Tag::createEmpty(),
                "Press Enter to start editing a Field")
        .addLabel({7, 25}, Tag::createEmpty(),
                "and Enter again to save changes.")
        .addLabel({10, 25}, Tag::createEmpty(),
                "Have fun!!!")
        ;

    m_Display.populateWindow(WindowType::Game)
        .addLabel({1, 2}, m_Tags.gameInfoWho, "")
        .addLabel({2, 2}, m_Tags.gameInfoConst, "is hanging")
        .addLabel({3, 2}, m_Tags.gameInfoWhom, "")
        .addLabel({4, 2}, m_Tags.gameTriesLeft, "")
        .addLabel({5, 2}, m_Tags.gameHint, "")
        .addLabel({2, 40}, m_Tags.gameStatus, "")
        ;


    m_Display.getLabelByTag(m_Tags.gameStatus)->get()
        .setAttribute(Drawer::Attribute::BOLD, true);
    m_Display.getLabelByTag(m_Tags.gameTriesLeft)->get()
        .setColor({Color::YELLOW, Color::BLACK})
        .changeTo("Tries left: " + std::to_string(Display::gallowsSteps));

    populateWithAlphabet(9);

    m_Display.setActiveWindow(WindowType::Settings);
    m_Display.disableWindow(WindowType::Game);
    // m_Display.setActiveWindow(WindowType::Game);
}

void Game::populateWithAlphabet(int yLevel) noexcept {
    auto& window = m_Display.populateWindow(WindowType::Game);
    const int WIDTH = 3;
    const int HEIGHT = 3;
    const int half = 13;
    const int xLevel = m_Display.getUiWidth() / 2 - half * WIDTH / 2;
    for (char c = 'A'; c <= 'Z'; c++) {
        const int index = c - 'A';
        const int shiftX = (index >= half) ? (index - half) : index;
        const int shiftY = (index >= half) ? 1 : 0;
        window.addButton(
            {yLevel + shiftY * HEIGHT, xLevel + shiftX * WIDTH},
            {HEIGHT, WIDTH},
            m_Tags.letters[index],
            std::string(1, c),
            [this, c](){
                tryLetter(c);
                checkEndgame();
            }
        );
    }
}

void Game::setupNewGame(
        bool hangingSelf,
        const std::string& selfNick,
        const std::string& opponentNick,
        std::string word) noexcept {

    // Make uppercase
    std::transform(word.begin(), word.end(), word.begin(),
        [](char c) -> char { return std::toupper(c); });

    // Create new context
    m_GameContext = {GameContext{hangingSelf, selfNick, opponentNick, word}};

    // Reset alphabet buttons
    for (char c = 'A'; c <= 'Z'; c++) {
        const unsigned int index = c - 'A';
        if (m_GameContext->hangingSelf) { // so self is playing
            m_Display.getButtonByTag(m_Tags.letters[index])->get().setStateColors({
                {Color::GREEN, Color::BLACK},
                {Color::BLACK, Color::GREEN},
                {Color::BLACK, Color::RED}
            }).setActive();
        } else { // so opponent is playing => I can't press buttons
            m_Display.getButtonByTag(m_Tags.letters[index])->get().setStateColors({
                {Color::GREEN, Color::BLACK},
                {Color::BLACK, Color::GREEN},
                {Color::BLACK, Color::RED}
            }).setPassive();
        }
    }

    // Set appropriate Labels
    if (m_GameContext->hangingSelf) {
        m_Display.getLabelByTag(m_Tags.gameInfoWho)->get()
            .changeTo(m_GameContext->opponentNick);
        m_Display.getLabelByTag(m_Tags.gameInfoWhom)->get()
            .changeTo(m_GameContext->selfNick);
        m_Display.getLabelByTag(m_Tags.gameHint)->get()
            .changeTo("Try your best!");
    } else {
        m_Display.getLabelByTag(m_Tags.gameInfoWho)->get()
            .changeTo(m_GameContext->selfNick);
        m_Display.getLabelByTag(m_Tags.gameInfoWhom)->get()
            .changeTo(m_GameContext->opponentNick);
        m_Display.getLabelByTag(m_Tags.gameHint)->get()
            .changeTo("Just watch!");
    }
    m_Display.getLabelByTag(m_Tags.gameStatus)->get().changeTo("");
    m_Display.getLabelByTag(m_Tags.gameTriesLeft)->get()
        .setColor({Color::YELLOW, Color::BLACK})
        .changeTo("Tries left: " + std::to_string(Display::gallowsSteps));

    m_Display.clearScreen();
}


void Game::markLetterAsUsed(char c, bool wrong) noexcept {
    if ('A' <= c && c <= 'Z') {
        const unsigned int index = c - 'A';
        m_Display.getButtonByTag(m_Tags.letters[index])->get().setStateColors({
            {Color::BLACK, wrong ? Color::RED : Color::YELLOW},
            {Color::BLACK, wrong ? Color::RED : Color::YELLOW},
            {Color::_, Color::_}
        }).setPassive();
    } else {
        Log::error().setClass("Game").setFunc("markLetterAsUsed")
            << "Called with invalid char:" << c << std::endl;
    }
}

void Game::tryLetter(char c) noexcept {
    if (!m_GameContext) {
        Log::error().setClass("Game").setFunc("tryLetter")
            << "Called outside of game context" << std::endl;
        return;
    }

    bool present = false;
    for (unsigned int i = 0; i < m_GameContext->word.size(); i++) {
        const char letter = m_GameContext->word[i];
        if (letter == c) {
            present = true;
            m_GameContext->revealed[i] = true;
        }
    }
    markLetterAsUsed(c, !present);
    if (!present) {
        m_GameContext->mistakesMade++;
        const auto triesLeft = Display::gallowsSteps - m_GameContext->mistakesMade;
        m_Display.getLabelByTag(m_Tags.gameTriesLeft)->get()
            // a trailing space to remove old leftovers but not use clearScreen()
            .changeTo("Tries left: " + std::to_string(triesLeft) + " ");
    }

    // Send to opponent if we're the ones playing
    if (m_GameContext->hangingSelf) {
        const std::string message =
            MessageTryLetter(c).asPacket();
        m_Communicator->sendAsync(m_DesiredOpponent->address, m_DesiredOpponent->port, message);
    }
}

void Game::checkEndgame() noexcept {
    if (!m_GameContext) {
        Log::error().setClass("Game").setFunc("checkEndgame")
            << "Called outside of game context" << std::endl;
        return;
    }

    const bool haveBeenHanged = [](const GameContext& context){
        return context.mistakesMade >= Display::gallowsSteps;
    }(*m_GameContext);

    const bool doneWithWord = [](const GameContext& context){
        for (bool revealed : context.revealed)
            if (!revealed) return false;
        return true;
    }(*m_GameContext);

    if (haveBeenHanged) {
        const bool lost = m_GameContext->hangingSelf;
        const std::string& result = lost ?
            "YOU LOST!" : "YOU WON!";
        m_Display.getLabelByTag(m_Tags.gameStatus)->get()
            .changeTo(result)
            .setColor({lost ? Color::RED : Color::GREEN, Color::BLACK});
    } else if (doneWithWord) {
        const bool lost = !m_GameContext->hangingSelf;
        const std::string& result = lost ?
            "YOU LOST!" : "YOU WON!";
        m_Display.getLabelByTag(m_Tags.gameStatus)->get()
            .changeTo(result)
            .setColor({lost ? Color::RED : Color::GREEN, Color::BLACK});
    }

    if (haveBeenHanged || doneWithWord) { // then the game has ended
        // disable buttons
        for (char c = 'A'; c <= 'Z'; c++) {
            const unsigned int index = c - 'A';
            m_Display.getButtonByTag(m_Tags.letters[index])->get().setPassive();
        }

        m_GameContext->ended = true;
    }
}


void Game::cleanup() {
    // TODO: need to finish Client/Server??
}

void Game::handleInput() {
    auto key = Keyboard::read();
    while (key) {
        processInputKey(*key);
        key = Keyboard::read();
    }
}

void Game::processInputKey(Key key) {
    /* if (key.is('q')) { */
    /*     m_Terminated = true; */
    /*     return; */
    /* } */

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
