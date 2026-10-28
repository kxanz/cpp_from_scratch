#include "Vector/vector.h"
#include <gtest/gtest.h>
#include <cstddef>
#include <utility>

// GTest::gtest_main supplies main() — no main() needed here.
//
// One TEST per feature. Fill each one in yourself.
//   EXPECT_EQ(a, b)    — records the failure and keeps going
//   ASSERT_EQ(a, b)    — stops this test on failure
//   EXPECT_TRUE(x) / EXPECT_FALSE(x), EXPECT_NE, EXPECT_LT, EXPECT_THROW(stmt, Type)
// Run one test:    ./build/tests/vector_/vector_test --gtest_filter=VectorTest.DefaultConstructorTest
// Run them all:    ./build/tests/vector_/vector_test

// Counts every way an object is created/destroyed. Call AllocCounter::reset()
// at the start of a test, then check the counters after using the vector.
struct AllocCounter
{
    static inline int default_ctor_count{ 0 };
    static inline int value_ctor_count{ 0 };
    static inline int copy_ctor_count{ 0 };
    static inline int move_ctor_count{ 0 };
    static inline int dtor_count{ 0 };
    static inline int alive{ 0 };
    int id{};

    AllocCounter() { ++default_ctor_count; ++alive; }
    explicit AllocCounter(int i) : id{ i } { ++value_ctor_count; ++alive; }
    AllocCounter(const AllocCounter& other) : id{ other.id } { ++copy_ctor_count; ++alive; }
    AllocCounter(AllocCounter&& other) noexcept : id{ other.id } { ++move_ctor_count; ++alive; }
    AllocCounter& operator=(const AllocCounter&) = default;
    AllocCounter& operator=(AllocCounter&&) noexcept = default;
    ~AllocCounter() { ++dtor_count; --alive; }

    static void reset()
    {
        default_ctor_count = 0;
        value_ctor_count = 0;
        copy_ctor_count = 0;
        move_ctor_count = 0;
        dtor_count = 0;
        alive = 0;
    }
};

struct NoDefault
{
    explicit NoDefault(int v) : value{ v } {}
    int value;
};

TEST(VectorTest, DefaultConstructorTest)
{
    kxanz::vector<int> v;

    EXPECT_EQ(v.empty(), true);
}

TEST(VectorTest, IntializerListTest)
{

}

TEST(VectorTest, DestructorTest)
{   
}

TEST(VectorTest, MoveConstructorTest)
{
}

TEST(VectorTest, MoveAssignmentTest)
{
}

TEST(VectorTest, GetSizeTest)
{
}

TEST(VectorTest, GetCapacityTest)
{
    kxanz::vector<int> v;
    std::cout << v.get_capacity() << '\n';
}

TEST(VectorTest, PushBackTest)
{
}

TEST(VectorTest, AtTest)
{
}

TEST(VectorTest, CapacityGrowthTest)
{
}

TEST(VectorTest, NoDefaultConstructTest)
{
}

TEST(VectorTest, PopBackTest)
{
}

TEST(VectorTest, ShrinkToFitTest)
{
}

TEST(VectorTest, SizeofTest)
{
}
