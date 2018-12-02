// Unit tests for classes of the Hangman project.

/* #include "gtest/gtest.h" */
#include "gmock/gmock.h"

#include "../src/utils.h"
#include "../src/Drawer.h"
#include "../src/Igorek.h"
#include "../src/Game.h"
#include "../src/Display.h"
#include "../src/Communicator.h"


using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;


class MockCommunicator : public Communicator {
    public:
        MOCK_METHOD0(receiveMessage, std::optional<std::string>());
        MOCK_METHOD1(sendMessage, bool(const std::string&));
        MOCK_METHOD2(establishConnection, bool(const std::string&, const std::string&));
        MOCK_METHOD0(connectionEstablished, bool());
};

class MockDrawer : public Drawer {
    public:
        MOCK_METHOD2(_move, void(unsigned int, unsigned int));
        MOCK_METHOD1(_addstr, void(const std::string&));
        MOCK_METHOD0(_flushinp, void());
        MOCK_METHOD1(_addch, void(int));
        MOCK_METHOD1(_attron, void(int));
        MOCK_METHOD1(_attroff, void(int));
        MOCK_METHOD0(_refresh, void());
        MOCK_METHOD0(_endwin, void());
        MOCK_METHOD0(_keypad, void());
        MOCK_METHOD0(_initscr, void());
        MOCK_METHOD0(_noecho, void());
        MOCK_METHOD0(_start_color, void());
        MOCK_METHOD0(_erase, void());

        MOCK_METHOD1(setColor, void(const Color&));
        MOCK_METHOD0(cleanup, void());
};


TEST(MockTest2, CommunicatorFunctions) {
    // MockDrawer mockDrawer;
    Renderer renderer(10,20);
    MockCommunicator mockCommunicator;

    EXPECT_CALL(mockCommunicator, connectionEstablished())
        .Times(AtLeast(4))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(mockCommunicator, establishConnection(_, _))
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(mockCommunicator, receiveMessage())
        .Times(AtLeast(1))
        .WillOnce(Return(std::optional<std::string>{std::string{"report"}}));
    EXPECT_CALL(mockCommunicator, sendMessage(_))
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false)); // Emulate internet connection loss

    Game game(&renderer, &mockCommunicator);

    EXPECT_FALSE(game.send("message without established connection"));
    EXPECT_TRUE(game.connect("192.162.0.1", "1234"));
    EXPECT_TRUE(game.send("message with connection established"));
    EXPECT_FALSE(game.send("won't transmit"));
    EXPECT_TRUE(game.receive());
    EXPECT_FALSE(game.connect("0.162.0.1", "1234"));
}


TEST (ButtonTest, IsUnderTest) {
    Display::Button button{{6,1}, {5, 12}, Tag::createNew(), "Press Me", [](){}};
    ASSERT_TRUE(button.isUnder({7,2}));
    ASSERT_FALSE(button.isUnder({13,20}));
}


TEST (CursorTest, InBoundsTest) {
    Display display{60, 100, new Renderer(10,10)};

    ASSERT_TRUE(display.setCursor({30, 30}));
    ASSERT_TRUE(display.setCursor({60, 99}));
    ASSERT_FALSE(display.setCursor({30, 101}));
}


TEST (WindowTest, IsInteructableTest) {
    Display::Window window{{0,0}, {60, 100}, {20, 30}, {Color::CYAN, Color::BLACK}};
    window.addLabel({{5, 2}, Tag::createNew(), "Test Window"})
        .addButton({{6, 1}, {5, 12}, Tag::createNew(), "Press Me", [](){}})
        .addField({{12, 1}, Tag::createNew(), 20, "Some field"});

    ASSERT_TRUE(window.getInteractableUnder({8, 5}));
    ASSERT_TRUE(window.getInteractableUnder({12, 19}));
    ASSERT_FALSE(window.getInteractableUnder({5, 3}));
}


TEST (CharDisplayTest, CheckIsLetter) {
    ASSERT_TRUE(isLetter('m'));
    ASSERT_FALSE(isLetter('&'));
    ASSERT_TRUE(isLetter('R'));
}


TEST (CharDisplayTest, CheckIsNumber) {
    ASSERT_FALSE(isNumber(0));
    ASSERT_FALSE(isNumber('n'));
    ASSERT_TRUE(isNumber('5'));
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


TEST (CharGameTest, CheckIsStandartChar) {
    ASSERT_TRUE(isStandartChar('q'));
    ASSERT_FALSE(isStandartChar(KEY_RIGHT));
    ASSERT_TRUE(isStandartChar('H'));
}


TEST (CharGameTest, CheckIsExtendedChar) {
    ASSERT_TRUE(isExtendedChar(KEY_DOWN));
    ASSERT_TRUE(isExtendedChar(KEY_RIGHT));
    ASSERT_FALSE(isExtendedChar('K'));
}


TEST (CharGameTest, CheckIsMovementChar) {
    ASSERT_TRUE(isMovementChar('k'));
    ASSERT_TRUE(isMovementChar(KEY_RIGHT));
    ASSERT_FALSE(isMovementChar('K'));
}


TEST (CharGameTest, CheckIsUpperCase) {
    ASSERT_FALSE(isUpperCase('k'));
    ASSERT_FALSE(isUpperCase(KEY_UP));
    ASSERT_TRUE(isUpperCase('K'));
}

// MOCKS HERE

TEST(MockTests, DrawerTest) {
    MockDrawer mockDrawer;
    MockCommunicator mockCommunicator;

    EXPECT_CALL(mockDrawer, _initscr())
        .Times(1);
    EXPECT_CALL(mockDrawer, _noecho())
        .Times(1);
    EXPECT_CALL(mockDrawer, _keypad())
        .Times(1);
    EXPECT_CALL(mockDrawer, _start_color())
        .Times(1);
    EXPECT_CALL(mockDrawer, _erase())
        .Times(AtLeast(1));

    Game game(&mockDrawer, &mockCommunicator);
    game.init();
}


int main(int argc, char **argv)
{
  printf("Running main() from main_test.cpp\n");

  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
