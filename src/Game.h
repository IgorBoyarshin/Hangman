#ifndef GAME_H
#define GAME_H

// For sleeping
#include <chrono>
#include <thread>

#include <algorithm> // transform()
#include <cctype> // toupper()

#include "Display.h"
#include "Renderer.h"
#include "Communicator.h"
#include "Keyboard.h"
#include "Key.h"
#include "Protocol.h"


struct GameTags {
    // inline GameTags()
    //     : selfNick(Tag::createNew()),
    //       oppoAddr(Tag::createNew()),
    //       oppoPort(Tag::createNew()),
    //       oppoWord(Tag::createNew()),
    //       connectButton(Tag::createNew()),
    //       acceptButton(Tag::createNew()),
    //       rejectButton(Tag::createNew()),
    //       exitButton(Tag::createNew()),
    //       disconnectButton(Tag::createNew()),
    //       connectionStatus(Tag::createNew()),
    //       oppoWants(Tag::createNew()) {}

    const Tag selfNick = Tag::createNew();
    const Tag oppoAddr = Tag::createNew();
    const Tag oppoPort = Tag::createNew();
    const Tag oppoWord = Tag::createNew();
    const Tag connectButton = Tag::createNew();
    const Tag acceptButton = Tag::createNew();
    const Tag rejectButton = Tag::createNew();
    const Tag exitButton = Tag::createNew();
    const Tag disconnectButton = Tag::createNew();
    const Tag connectionStatus = Tag::createNew();
    const Tag oppoWants = Tag::createNew();
    const Tag gameInfoWho = Tag::createNew();
    const Tag gameInfoConst = Tag::createNew();
    const Tag gameInfoWhom = Tag::createNew();
    const Tag gameStatus = Tag::createNew();
    const Tag gameHint = Tag::createNew();
    const std::vector<Tag> letters = [](){
        std::vector<Tag> tags;
        for (char c = 'A'; c <= 'Z'; c++) tags.push_back(Tag::createNew());
        return tags;
    }();
};


struct InquiringOpponent {
    inline InquiringOpponent() {}
    inline InquiringOpponent(
            const std::string& nick, const std::string& address,
            unsigned int port, const std::string& word)
        : nick(nick), address(address), port(port), word(word) {}

    std::string nick;
    std::string address;
    unsigned int port;
    std::string word;
};

struct DesiredOpponent {
    inline DesiredOpponent() {}
    inline DesiredOpponent(
            const std::string& address,
            unsigned int port, const std::string& word)
        : address(address), port(port), word(word) {}

    std::string address;
    unsigned int port;
    std::string word;
};

struct GameContext {
    // inline InquiringOpponent() {}
    inline GameContext(
            bool hangingSelf,
            const std::string& selfNick,
            const std::string& opponentNick,
            const std::string& word
            /* const std::string& address, */
            /* unsigned int port, */
        )
        :
        hangingSelf(hangingSelf),
        mistakesMade(0),
        selfNick(selfNick),
        opponentNick(opponentNick),
        word(word),
        revealed(std::vector<bool>(word.size(), false)),
        ended(false)
        // address(address),
        // port(port),
    {}

    bool hangingSelf;
    unsigned int mistakesMade;
    std::string selfNick;
    std::string opponentNick;
    /* std::string address; */
    /* unsigned int port; */
    std::string word;
    std::vector<bool> revealed;
    bool ended;
};


class Game {
    private:
        Display m_Display;
        Communicator* m_Communicator;
        bool m_Terminated;
        const GameTags m_Tags;
        InquiringOpponent m_InquiringOpponent;
        std::optional<DesiredOpponent> m_DesiredOpponent;
        std::optional<GameContext> m_GameContext;

    public:
        Game(unsigned int width, unsigned int height,
                Drawer* drawer, Communicator* communicator);
        virtual ~Game();
        void init();
        void loop();
    private:
        void initDisplay();
        void populateWithAlphabet(int yLevel) noexcept;
        void setupNewGame(
                bool hangingSelf,
                const std::string& selfNick,
                const std::string& opponentNick,
                std::string word) noexcept;
        void cleanup();
        void handleInput();
        void handleIncoming();
        void processInputKey(Key key);
        void handleWannaPlay(const MessageWannaPlay& message) noexcept;
        void handleAcceptedPlay(const MessageAcceptedPlay& message) noexcept;
        void handleRejectedPlay(const MessageRejectedPlay& message) noexcept;
        void handlePlayNoMore(const MessagePlayNoMore& message) noexcept;

        void processConnectPress() noexcept;
        void processAcceptPress() noexcept;
        void processRejectPress() noexcept;
        void processDisconnectPress() noexcept;
        void processExitPress() noexcept;

        void markLetterAsUsed(char c, bool wrong) noexcept;
        void tryLetter(char c) noexcept;
        void checkEndgame() noexcept;
        /* void fillCorrectAnswer() noexcept; */
};


#endif
