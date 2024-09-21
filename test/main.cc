#include "macro.h"
#include "DynArr.h"

int main(int argc, char** argv) {
    auto dynarr = FuncDoodle::LongIndexArray<int>();

    ASSERT_EQ(dynarr.getSize(), 0);
    ASSERT_EQ(dynarr.getCapacity(), 10);

    dynarr.push_back(9);

    ASSERT_EQ(dynarr.getSize(), 1);
    ASSERT_EQ(dynarr.getCapacity(), 10);

    dynarr.push_back(2);

    ASSERT_EQ(dynarr.getSize(), 2);
    ASSERT_EQ(dynarr.getCapacity(), 10);

    dynarr.push_back(3);

    ASSERT_EQ(dynarr.getSize(), 3);
    ASSERT_EQ(dynarr.getCapacity(), 10);
}
