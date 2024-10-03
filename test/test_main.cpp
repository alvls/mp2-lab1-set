#include <gtest.h>
#include "tbitfield.h"
#include "tset.h"

int main(int argc, char **argv) {
    const int size = 3;
    TBitField bf1(size);
    TBitField bf2(size);
    bf1.ClrBit(2);
    bf2.ClrBit(2);

    EXPECT_NE(bf1.GetBit(2), bf1.GetBit(2));

    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
}
