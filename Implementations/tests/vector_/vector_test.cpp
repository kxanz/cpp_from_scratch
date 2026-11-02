#include "Vector/vector.h"
#include <gtest/gtest.h>
#include <cstddef>
#include <iostream>
#include <stdexcept>
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

template <typename T>
void print(kxanz::vector<T>& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}

TEST(VectorTest, DefaultConstructorTest)
{
    kxanz::vector<int> v;

    EXPECT_EQ(v.empty(), true);
    EXPECT_EQ(v.get_size(), 0);
    EXPECT_EQ(v.get_capacity(), 1);
}

TEST(VectorTest, IntializerListTest)
{
    kxanz::vector<int> listNum {1, 2, 3, 4, 5};

    print(listNum);

    EXPECT_EQ(listNum.get_size(), 5);
    EXPECT_EQ(listNum.get_capacity(), 5);
    for (std::size_t i {0}; i < listNum.get_size(); ++i)
    {
        EXPECT_EQ(listNum.at(i), static_cast<int>(i) + 1);
    }
}

TEST(VectorTest, DestructorTest)
{
    AllocCounter::reset();
    {
        kxanz::vector<AllocCounter> v;
        v.push_back(AllocCounter{ 1 });
        v.push_back(AllocCounter{ 2 });
        v.push_back(AllocCounter{ 3 });
        EXPECT_GT(AllocCounter::alive, 0);
    }
    EXPECT_EQ(AllocCounter::alive, 0);
}

TEST(VectorTest, CopyConstructorTest)
{
    kxanz::vector<int> a {1, 2, 3};
    kxanz::vector<int> b(a);

    print(b);

    ASSERT_EQ(b.get_size(), 3);
    EXPECT_EQ(b.at(0), 1);
    EXPECT_EQ(b.at(2), 3);
    EXPECT_EQ(a.get_size(), 3);
    EXPECT_EQ(a.at(0), 1);
}

TEST(VectorTest, CopyAssignmentTest)
{
    kxanz::vector<int> a {1, 2, 3};
    kxanz::vector<int> b {7, 8};
    b = a;

    print(b);

    ASSERT_EQ(b.get_size(), 3);
    EXPECT_EQ(b.at(0), 1);
    EXPECT_EQ(b.at(2), 3);
    EXPECT_EQ(a.get_size(), 3);
}

TEST(VectorTest, MoveConstructorTest)
{
    kxanz::vector<AllocCounter> a;
    a.push_back(AllocCounter{ 1 });
    a.push_back(AllocCounter{ 2 });

    AllocCounter::reset();
    kxanz::vector<AllocCounter> b(std::move(a));

    EXPECT_EQ(b.get_size(), 2);
    EXPECT_EQ(b.at(0).id, 1);
    EXPECT_EQ(b.at(1).id, 2);
    EXPECT_EQ(a.get_size(), 0);
    EXPECT_EQ(a.get_capacity(), 0);
    EXPECT_EQ(AllocCounter::copy_ctor_count, 0);
    EXPECT_EQ(AllocCounter::move_ctor_count, 0);
}

TEST(VectorTest, MoveAssignmentTest)
{
    kxanz::vector<int> a {1, 2, 3};
    kxanz::vector<int> b {7, 8};
    b = std::move(a);

    print(b);

    EXPECT_EQ(b.get_size(), 3);
    EXPECT_EQ(b.at(0), 1);
    EXPECT_EQ(b.at(2), 3);
    EXPECT_EQ(a.get_size(), 0);
    EXPECT_EQ(a.get_capacity(), 0);
}

TEST(VectorTest, GetSizeTest)
{
    kxanz::vector<int> v(3, 7);
    EXPECT_EQ(v.get_size(), 3);

    v.push_back(8);
    EXPECT_EQ(v.get_size(), 4);

    v.pop_back();
    EXPECT_EQ(v.get_size(), 3);
}

TEST(VectorTest, GetCapacityTest)
{
    kxanz::vector<int> v {1,2,3};
    std::cout << v.get_capacity() << '\n';

    EXPECT_EQ(v.get_capacity(), 3);

    v.push_back(4);
    EXPECT_EQ(v.get_capacity(), 6);
}

TEST(VectorTest, PushBackTest)
{
    kxanz::vector<int> vec { 1, 2, 3, 4, 5, };
    
    auto i { 2 };
    vec.push_back(i);
    print(vec);

    EXPECT_EQ(vec.get_size(), 6);
    EXPECT_EQ(vec.at(5), 2);
    EXPECT_EQ(vec.at(0), 1);
    EXPECT_EQ(vec.at(4), 5);
}

TEST(VectorTest, AtTest)
{
    kxanz::vector<int> v {10, 20, 30};

    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST(VectorTest, CapacityGrowthTest)
{
    kxanz::vector<int> v(std::size_t{ 0 });
    EXPECT_EQ(v.get_capacity(), 0);

    const std::size_t expected[] {1, 2, 4, 4, 8, 8, 8, 8, 16, 16};
    for (int i {0}; i < 10; ++i)
    {
        v.push_back(i);
        EXPECT_EQ(v.get_capacity(), expected[i]);
    }
    EXPECT_EQ(v.get_size(), 10);
    EXPECT_EQ(v.at(9), 9);
}

TEST(VectorTest, NoDefaultConstructTest)
{
    kxanz::vector<NoDefault> v;
    v.push_back(NoDefault{ 1 });
    v.push_back(NoDefault{ 2 });

    EXPECT_EQ(v.get_size(), 2);
    EXPECT_EQ(v.at(0).value, 1);
    EXPECT_EQ(v.at(1).value, 2);
}

TEST(VectorTest, PopBackTest)
{
    kxanz::vector<AllocCounter> v;
    v.push_back(AllocCounter{ 1 });
    v.push_back(AllocCounter{ 2 });

    AllocCounter::reset();
    v.pop_back();
    EXPECT_EQ(v.get_size(), 1);
    EXPECT_EQ(AllocCounter::dtor_count, 1);
    EXPECT_EQ(v.at(0).id, 1);

    v.pop_back();
    EXPECT_TRUE(v.empty());

    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, ShrinkToFitTest)
{
    kxanz::vector<int> v {1, 2, 3, 4, 5};
    v.push_back(6);
    EXPECT_EQ(v.get_capacity(), 10);

    v.shrink_to_fit();
    EXPECT_EQ(v.get_capacity(), 6);
    EXPECT_EQ(v.get_size(), 6);
    print(v);

    v.shrink_to_fit();
    EXPECT_EQ(v.get_capacity(), 6);
}

TEST(VectorTest, SizeofTest)
{
    EXPECT_EQ(sizeof(kxanz::vector<int>), 3 * sizeof(void*));
}
