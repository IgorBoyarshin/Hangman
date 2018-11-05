// Unit tests for classes of the Hangman project.

#include "gtest/gtest.h"
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
