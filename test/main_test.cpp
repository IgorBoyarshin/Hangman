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


int main(int argc, char **argv)
{
  printf("Running main() from main_test.cpp\n");

  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
