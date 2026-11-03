#include "SharedPtr/sharedptr.h"
#include <gtest/gtest.h>
#include <string>
#include <utility>

// GTest::gtest_main supplies main() — no main() needed here.
//
// One TEST per required test case, plus make_shared. Fill each one in yourself.
//   EXPECT_EQ(a, b)    — records the failure and keeps going
//   ASSERT_EQ(a, b)    — stops this test on failure
//   EXPECT_TRUE(x) / EXPECT_FALSE(x), EXPECT_NE, EXPECT_LT, EXPECT_THROW(stmt, Type)
// Run one test:    GTEST_FILTER=SharedPointerTest.ScopingTest cmakerun sharedptr_test
// Run them all:    cmakerun sharedptr_test

// Counts live objects. Call Tracker::reset() at the start of a test, then
// check Tracker::alive to see whether the owned object was deleted.
struct Tracker
{
    static inline int alive{ 0 };
    static inline int dtor_count{ 0 };
    int id{};

    explicit Tracker(int i) : id{ i } { ++alive; }
    ~Tracker() { ++dtor_count; --alive; }

    static void reset()
    {
        alive = 0;
        dtor_count = 0;
    }
};

// Takes two constructor arguments, for testing make_shared's forwarding.
struct Point
{
    Point(int x_, std::string name_) : x{ x_ }, name{ std::move(name_) } {}
    int x;
    std::string name;
};

TEST(SharedPointerTest, ScopingTest)
{
}

TEST(SharedPointerTest, SizeTest)
{
}

TEST(SharedPointerTest, MoveConstructTest)
{
}

TEST(SharedPointerTest, MoveAssignTest)
{
}

TEST(SharedPointerTest, ResetTest)
{
}

TEST(SharedPointerTest, BoolTest)
{
}

TEST(SharedPointerTest, ArrowTest)
{
}

TEST(SharedPointerTest, DereferenceTest)
{
}

TEST(SharedPointerTest, CopyAssignTest)
{
}

TEST(SharedPointerTest, CopyConstructTest)
{
}

TEST(SharedPointerTest, MakeSharedTest)
{
}
