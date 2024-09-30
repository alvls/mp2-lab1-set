#include <gtest.h>

TEST(SimpleTest, sum_eq_prod_1) {
    EXPECT_EQ(1 + 3 + 5 + 7 + 9, 5 * 5);
}

TEST(SimpleTest, sum_eq_prod_2) {
    EXPECT_EQ(1 + 3 + 5 + 7 + 9 + 11, 6 * 6);
}