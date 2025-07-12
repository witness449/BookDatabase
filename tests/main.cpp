#include <gtest/gtest.h>
#include <print>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::print("Hello book");
    return RUN_ALL_TESTS();
}
