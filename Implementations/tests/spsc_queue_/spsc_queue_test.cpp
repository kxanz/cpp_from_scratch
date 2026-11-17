#include "SPSCQueue/spsc_queue.h"
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

// GTest::gtest_main supplies main() — no main() needed here.
//
// One TEST per requirement, plus the example from the problem. Fill each one in yourself.
//   EXPECT_EQ(a, b)    — records the failure and keeps going
//   ASSERT_EQ(a, b)    — stops this test on failure
//   EXPECT_TRUE(x) / EXPECT_FALSE(x), EXPECT_NE, EXPECT_LT, EXPECT_THROW(stmt, Type)
// Run one test:    GTEST_FILTER=SPSCTest.ExampleTest cmakerun spsc_queue_test
// Run them all:    cmakerun spsc_queue_test

TEST(SPSCTest, ExampleTest)
{
}

TEST(SPSCTest, CallbackTest)
{
}

TEST(SPSCTest, OrderTest)
{
}

TEST(SPSCTest, LastChunkTest)
{
}

TEST(SPSCTest, UnboundedTest)
{
}

TEST(SPSCTest, CleanupTest)
{
}

TEST(SPSCTest, CopyMoveTest)
{
}
