// Unit tests for classes of the Hangman project.

#include "gtest/gtest.h"
#include "gmock/gmock.h"

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



int main(int argc, char **argv)
{
  printf("Running main() from main_test.cpp\n");

  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
