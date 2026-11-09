#include "Allocator/allocator.h"
#include <gtest/gtest.h>
#include <cstddef>
#include <new>

// GTest::gtest_main supplies main() — no main() needed here.
//
// One TEST per required test case, plus AlignmentTest. Fill each one in yourself.
//   EXPECT_EQ(a, b)    — records the failure and keeps going
//   ASSERT_EQ(a, b)    — stops this test on failure
//   EXPECT_TRUE(x) / EXPECT_FALSE(x), EXPECT_NE, EXPECT_LT, EXPECT_THROW(stmt, Type)
// Run one test:    GTEST_FILTER=AllocatorTest.AllocateTest cmakerun allocator_test
// Run them all:    cmakerun allocator_test

TEST(AllocatorTest, DeallocateTest)
{
}

TEST(AllocatorTest, DeallocateUnknownTest)
{
}

TEST(AllocatorTest, AllocateTest)
{
}

TEST(AllocatorTest, AllocateTooBigTest)
{
}

TEST(AllocatorTest, SizeTest)
{
}

TEST(AllocatorTest, AllocateEmptyTest)
{
}

TEST(AllocatorTest, DeallocateEmptyTest)
{
}

TEST(AllocatorTest, CanYouReadInstructionsTest)
{
}

TEST(AllocatorTest, AlignmentTest)
{
}
