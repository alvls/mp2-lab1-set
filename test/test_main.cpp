#include <gtest.h>
#include "tbitfield.h"
#include "tset.h"

int main(int argc, char **argv) {
    TBitField a(6);
    cin >> a;
    cout << a;
    cout << endl << ~a;
    system("pause");
    cout << "=============================" << endl;
    TSet b(10);
    cin >> b;
    cout << b;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
