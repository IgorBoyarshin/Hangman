// Unit tests for classes of Hangman project.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* #include "../lib/googletest/googletest/include/gtest/gtest.h" */
/* #include "../lib/googletest/googlemock/include/gmock/gmock.h" */

#include "../src/Igorek.h"


using ::testing::Return;
using ::testing::_;

class MockIgorek : public Igorek {
    public:
        MOCK_METHOD2(mul, int(int, int));
};

// class MockButton : public Display::Button {
//     public:
//         MOCK_METHOD0(handleCursorOver, void() noexcept);
//         MOCK_METHOD0(handleCursorAway, void() noexcept);
//         MOCK_METHOD1(isUnder, bool(const Coord&) const noexcept);
// };
//
//
// TEST (ButtonTest, ProcessesInputs) {
//     MockButton button;
//
//     Display display;
//     // display.populateWindow(WindowType::GAME)
//
//         // .addButton({{6,1}, {5, 12}, Display::Tag::createNew(), "Press Me", [](){});
// }

TEST (SumTest, PositiveNums) {
    EXPECT_EQ (8, sum(3, 5));
    EXPECT_NE (4, sum(0, 3));

    MockIgorek mockIgorek;
    EXPECT_CALL(mockIgorek, mul(_, 3))
        .WillOnce(Return(6));

    // Igorek igorek;
    /* const int res1 = igorek.mul(2,3); */
    const int res2 = mockIgorek.mul(3,3);

    EXPECT_TRUE(6 == res2);
}


int main(int argc, char **argv)
{
  printf("Running main() from main_test.cpp\n");

  // testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
