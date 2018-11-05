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
