#ifndef GAME_H
#define GAME_H

// For sleeping
#include <chrono>
#include <thread>

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
};


struct PotentialOpponent {
    inline PotentialOpponent() {}
    inline PotentialOpponent(
            const std::string& nick, const std::string& address,
            unsigned int port, const std::string& word)
        : nick(nick), address(address), port(port), word(word) {}

    std::string nick;
    std::string address;
    unsigned int port;
    std::string word;
};


class Game {
    private:
        Display m_Display;
        Communicator* m_Communicator;
        bool m_Terminated;
        const GameTags m_Tags;
        PotentialOpponent m_PotentialOpponent;

    public:
        Game(unsigned int width, unsigned int height,
                Drawer* drawer, Communicator* communicator);
        virtual ~Game();
        void init();
        void loop();
    private:
        void initDisplay();
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
};


#endif
