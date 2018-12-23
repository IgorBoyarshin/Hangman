// Unit tests for classes of the Hangman project.

/* #include "gtest/gtest.h" */
#include "gmock/gmock.h"

#include "../src/utils.h"
#include "../src/Drawer.h"
#include "../src/Game.h"
#include "../src/Display.h"
#include "../src/Communicator.h"
#include "../src/NetworkManager.h"
#include "../src/Key.h"


using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;


class MockCommunicator : public Communicator {
    public:
        MOCK_METHOD0(receive, std::optional<std::string>());
        MOCK_METHOD0(blockReceive, std::string());
        MOCK_METHOD3(send, bool(const std::string&, unsigned int, const std::string&));
        MOCK_METHOD2(establishConnection, bool(const std::string&, const std::string&));
        MOCK_METHOD0(connectionEstablished, bool());
        MOCK_CONST_METHOD0(getAddress, std::string());
        MOCK_CONST_METHOD0(getPort, unsigned int());
};

class MockDrawer : public Drawer {
    public:
        MOCK_METHOD2(goTo, void(unsigned int, unsigned int));
        MOCK_METHOD1(put, void(const std::string&));
        MOCK_METHOD1(put, void(char));
        MOCK_METHOD1(put, void(SpecialChar));
        MOCK_METHOD2(setAttribute, void(Attribute, bool));
        /* MOCK_METHOD1(_attron, void(int)); */
        /* MOCK_METHOD1(_attroff, void(int)); */
        MOCK_METHOD0(update, void());
        MOCK_METHOD0(init, void());
        MOCK_METHOD0(enableColors, void());
        MOCK_METHOD0(clearScreen, void());
        MOCK_METHOD1(setColor, void(const Color&));
        MOCK_METHOD0(cleanup, void());
};
// ----------------------------------------------------------------------------
// ---------------- Mock Tests
// ----------------------------------------------------------------------------
TEST(IntegrationTest, NetworkConnectionTest) {
    // Expects a server to be running (It is launched with Docker)
    /* const std::string serverAddr = "172.17.0.2"; */
    const std::string serverAddr = "127.0.0.1";
    const unsigned int serverPort = 8080;

    const std::string name = "ConnectingPlayer";
    /* const std::string addr = "172.17.0.3"; */
    const std::string addr = "127.0.0.1";
    const unsigned int port = 1234;
    Communicator* communicator = new NetworkManager(name, addr, port);

    const std::string message1 = "hello from " + addr + " " + std::to_string(port);
    const bool sendSuccessful1 = communicator->send(serverAddr, serverPort, message1);
    EXPECT_TRUE(sendSuccessful1);
    if (sendSuccessful1) {
        const std::string reply = communicator->blockReceive();
        EXPECT_EQ(reply, "hello to you too, good sir!!");
    }

    const std::string message2 = "who are you " + addr + " " + std::to_string(port);
    const bool sendSuccessful2 = communicator->send(serverAddr, serverPort, message2);
    EXPECT_TRUE(sendSuccessful2);
    if (sendSuccessful2) {
        const std::string reply = communicator->blockReceive();
        const std::string beg = reply.substr(0, 4);
        EXPECT_TRUE(beg == "I am");
    }

    delete communicator;
}

// TEST(MockTest2, CommunicatorFunctions) {
//     Renderer renderer;
//     MockCommunicator mockCommunicator;
//
//     EXPECT_CALL(mockCommunicator, connectionEstablished())
//         .Times(AtLeast(4))
//         .WillOnce(Return(false))
//         .WillRepeatedly(Return(true));
//     EXPECT_CALL(mockCommunicator, establishConnection(_, _))
//         .WillOnce(Return(true))
//         .WillOnce(Return(false));
//     EXPECT_CALL(mockCommunicator, receiveMessage())
//         .Times(AtLeast(1))
//         .WillOnce(Return(std::optional<std::string>{std::string{"report"}}));
//     EXPECT_CALL(mockCommunicator, sendMessage(_))
//         .WillOnce(Return(true))
//         .WillRepeatedly(Return(false)); // Emulate internet connection loss
//
//     Game game(&renderer, &mockCommunicator);
//
//     EXPECT_FALSE(game.send("message without established connection"));
//     EXPECT_TRUE(game.connect("192.162.0.1", "1234"));
//     EXPECT_TRUE(game.send("message with connection established"));
//     EXPECT_FALSE(game.send("won't transmit"));
//     EXPECT_TRUE(game.receive());
//     EXPECT_FALSE(game.connect("0.162.0.1", "1234"));
// }


TEST(MockTests, DrawerTest) {
    MockDrawer mockDrawer;
    MockCommunicator mockCommunicator;

    EXPECT_CALL(mockDrawer, init())
        .Times(1);
    EXPECT_CALL(mockDrawer, enableColors())
        .Times(1);
    EXPECT_CALL(mockDrawer, clearScreen())
        .Times(AtLeast(1));

    Game game(60, 20, &mockDrawer, &mockCommunicator);
    game.init();
}
// ----------------------------------------------------------------------------
// ---------------- Regular Tests
// ----------------------------------------------------------------------------
TEST (ButtonTest, IsUnderTest) {
    Display::Button button{{6,1}, {5, 12}, Tag::createNew(), "Press Me", [](){}};
    ASSERT_TRUE(button.isUnder({7,2}));
    ASSERT_FALSE(button.isUnder({13,20}));
}


TEST (CursorTest, InBoundsTest) {
    Display display{60, 100, new Renderer()};

    ASSERT_TRUE(display.setCursor({30, 30}));
    ASSERT_TRUE(display.setCursor({60, 99}));
    ASSERT_FALSE(display.setCursor({30, 101}));
}


TEST (WindowTest, IsInteractableTest) {
    Display::Window window{{0,0}, {60, 100}, {20, 30}, {Color::CYAN, Color::BLACK}};
    window.addLabel({5, 2}, Tag::createNew(), "Test Window")
        .addButton({6, 1}, {5, 12}, Tag::createNew(), "Press Me", [](){})
        .addField({12, 1}, Tag::createNew(), 20, "Some field");

    const unsigned int shiftY = 4;
    const unsigned int shiftX = 1;
    ASSERT_TRUE(window.getInteractableUnder({8 + shiftY, 5 + shiftX}));
    ASSERT_TRUE(window.getInteractableUnder({12 + shiftY, 19 + shiftX}));
    ASSERT_FALSE(window.getInteractableUnder({5 + shiftY, 3 + shiftX}));
}


TEST (CharDisplayTest, CheckIsLetter) {
    ASSERT_TRUE(Key('m').isLetter());
    ASSERT_FALSE(Key('&').isLetter());
    ASSERT_TRUE(Key('R').isLetter());
}


TEST (CharDisplayTest, CheckIsNumber) {
    ASSERT_FALSE(Key(0).isNumber());
    ASSERT_FALSE(Key('n').isNumber());
    ASSERT_TRUE(Key('5').isNumber());
}


TEST (TagTest, EmptyTagTest) {
    Tag emptyTag{Tag::createEmpty()};

    ASSERT_EQ(0, emptyTag());
}


TEST (TagTest, TwoTagsTest) {
    Tag tag1{Tag::createNew()};
    Tag tag2{Tag::createNew()};

    ASSERT_NE(0, tag2());
    ASSERT_NE(tag1(), tag2());
    ASSERT_EQ(tag1() + 1, tag2());
}


TEST (KeyTest, CheckIsRegular) {
    ASSERT_TRUE(Key('q').isRegular());
    ASSERT_FALSE(Key(KEY_RIGHT).isRegular());
    ASSERT_TRUE(Key(123).isRegular());
}


TEST (KeyTest, CheckIsSpecial) {
    ASSERT_TRUE(Key(KEY_DOWN).isSpecial());
    ASSERT_TRUE(Key(KEY_RIGHT).isSpecial());
    ASSERT_FALSE(Key('K').isSpecial());
}


TEST (KeyTest, CheckKeyIs) {
    ASSERT_TRUE(Key('a').is('a'));
    ASSERT_TRUE(Key(KEY_DOWN).is(Key::SpecialKey::DOWN));
    ASSERT_FALSE(Key('b').is(Key::SpecialKey::DOWN));
}


TEST (KeyTest, CheckDirection) {
    ASSERT_TRUE(Key('j').asDirection());
    ASSERT_EQ(*Key('j').asDirection(), Direction::DOWN);
    ASSERT_EQ(*Key(KEY_DOWN).asDirection(), Direction::DOWN);
    ASSERT_FALSE(Key('u').asDirection());
}


int main(int argc, char **argv) {
    printf("Running main() from main_test.cpp\n");

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
