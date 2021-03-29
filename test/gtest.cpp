#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gcp.h"
#include <limits>


TEST(garbage_collector_tests, create_gc_ptr)
{
    GCP<int> ptr(new int(77));
    bool t = *ptr == 77;

    ASSERT_TRUE(t);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}