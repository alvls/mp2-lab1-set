
#include "gtest.h"

// Тест 1: 1 + 3 + 5 + 7 + 9 == 5^2
TEST(SimpleMathTest, SumFirst5OddsEquals25) {
    EXPECT_EQ(1 + 3 + 5 + 7 + 9, 25);
    EXPECT_EQ(1 + 3 + 5 + 7 + 9, 5 * 5);
}

// Тест 2: 1 + 3 + 5 + 7 + 9 + 11 == 6^2
TEST(SimpleMathTest, SumFirst6OddsEquals36) {
    EXPECT_EQ(1 + 3 + 5 + 7 + 9 + 11, 36);
    EXPECT_EQ(1 + 3 + 5 + 7 + 9 + 11, 6 * 6);
}

// Тест 3 (необязательно): то же самое, но выражение вычисляется в теле теста
TEST(SimpleMathTest, SumFirst7OddsEquals49) {
    int sum = 1 + 3 + 5 + 7 + 9 + 11 + 13;
    EXPECT_EQ(sum, 49);   // 7^2
}