#include "Vector/vector.h"
#include <catch2/catch_test_macros.hpp>
#include <cstddef>

// Catch2::Catch2WithMain supplies main() — no CATCH_CONFIG_MAIN needed.
//

// Counts every way an object is created/destroyed, so tests can prove
// the vector never default-constructs spare capacity and destroys exactly what it built.
struct Tracker
{
    static inline int default_constructed{ 0 };
    static inline int value_constructed{ 0 };
    static inline int copied{ 0 };
    static inline int moved{ 0 };
    static inline int destroyed{ 0 };
    static inline int alive{ 0 };
    int id{};

    Tracker() { ++default_constructed; ++alive; }
    explicit Tracker(int i) : id{ i } { ++value_constructed; ++alive; }
    Tracker(const Tracker& other) : id{ other.id } { ++copied; ++alive; }
    Tracker(Tracker&& other) noexcept : id{ other.id } { ++moved; ++alive; }
    Tracker& operator=(const Tracker&) = default;
    Tracker& operator=(Tracker&&) noexcept = default;
    ~Tracker() { ++destroyed; --alive; }
};

// No default constructor at all — if the vector default-constructs spare slots,
// this file stops compiling.
struct NoDefault
{
    explicit NoDefault(int v) : value{ v } {}
    int value;
};

// Catch2 re-runs the TEST_CASE body from the top for every SECTION,
// so resetting here gives each SECTION fresh counters.
static void reset_counters()
{
    Tracker::default_constructed = 0;
    Tracker::value_constructed = 0;
    Tracker::copied = 0;
    Tracker::moved = 0;
    Tracker::destroyed = 0;
    Tracker::alive = 0;
}

TEST_CASE("kxanz::vector size and capacity", "[vector]")
{
    SECTION("A new vector has size 0 and capacity 1")
    {
        kxanz::vector<int> v;
        REQUIRE(v.get_size() == 0);
        REQUIRE(v.get_capacity() == 1);
    }

    SECTION("First push_back fits in the initial capacity")
    {
        kxanz::vector<int> v;
        v.push_back(10);
        REQUIRE(v.get_size() == 1);
        REQUIRE(v.get_capacity() == 1);
    }

    SECTION("Capacity triples every time it is reached: 1 -> 3 -> 9 -> 27")
    {
        kxanz::vector<int> v;
        v.push_back(0);
        REQUIRE(v.get_capacity() == 1);
        v.push_back(1);
        REQUIRE(v.get_capacity() == 3);
        v.push_back(2);
        REQUIRE(v.get_capacity() == 3);
        v.push_back(3);
        REQUIRE(v.get_capacity() == 9);
        for (int i = 4; i < 10; ++i) { v.push_back(i); }
        REQUIRE(v.get_size() == 10);
        REQUIRE(v.get_capacity() == 27);
    }

    SECTION("sizeof stays small: one pointer plus two counts")
    {
        REQUIRE(sizeof(kxanz::vector<int>) <= sizeof(void*) + 2 * sizeof(std::size_t));
    }
}

TEST_CASE("push_back and at", "[vector]")
{
    SECTION("at() returns each pushed element in order")
    {
        kxanz::vector<int> v;
        for (int i = 0; i < 20; ++i) { v.push_back(i * 10); }
        for (std::size_t i = 0; i < 20; ++i)
        {
            REQUIRE(v.at(i) == static_cast<int>(i) * 10);
        }
    }

    SECTION("Elements survive every growth step")
    {
        kxanz::vector<int> v;
        v.push_back(7);
        v.push_back(8);
        v.push_back(9);
        v.push_back(10);
        REQUIRE(v.at(0) == 7);
        REQUIRE(v.at(3) == 10);
    }

    SECTION("at() works on a const vector")
    {
        kxanz::vector<int> v;
        v.push_back(42);
        const kxanz::vector<int>& cv = v;
        REQUIRE(cv.at(0) == 42);
    }

    SECTION("Works with a type that has no default constructor")
    {
        kxanz::vector<NoDefault> v;
        v.push_back(NoDefault{ 1 });
        v.push_back(NoDefault{ 2 });
        REQUIRE(v.at(1).value == 2);
    }
}

TEST_CASE("pop_back and shrink_to_fit", "[vector]")
{
    SECTION("pop_back removes the last element and keeps capacity")
    {
        kxanz::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        v.pop_back();
        REQUIRE(v.get_size() == 3);
        REQUIRE(v.get_capacity() == 9);
        REQUIRE(v.at(2) == 3);
    }

    SECTION("push_back after pop_back reuses the slot")
    {
        kxanz::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.pop_back();
        v.push_back(5);
        REQUIRE(v.get_size() == 2);
        REQUIRE(v.at(1) == 5);
    }

    SECTION("shrink_to_fit makes capacity equal size and keeps the elements")
    {
        kxanz::vector<int> v;
        for (int i = 0; i < 4; ++i) { v.push_back(i); }
        REQUIRE(v.get_capacity() == 9);
        v.shrink_to_fit();
        REQUIRE(v.get_capacity() == 4);
        REQUIRE(v.get_size() == 4);
        REQUIRE(v.at(0) == 0);
        REQUIRE(v.at(3) == 3);
    }

    SECTION("shrink_to_fit when already full changes nothing")
    {
        kxanz::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.shrink_to_fit();
        REQUIRE(v.get_capacity() == 3);
        REQUIRE(v.at(2) == 3);
    }

    SECTION("push_back after shrink_to_fit grows again by tripling")
    {
        kxanz::vector<int> v;
        for (int i = 0; i < 4; ++i) { v.push_back(i); }
        v.shrink_to_fit();
        v.push_back(4);
        REQUIRE(v.get_capacity() == 12);
        REQUIRE(v.at(4) == 4);
    }
}

TEST_CASE("Tracker object: construction and destruction counts", "[vector]")
{
    reset_counters();

    SECTION("An empty vector constructs no elements")
    {
        kxanz::vector<Tracker> v;
        REQUIRE(v.get_size() == 0);
        REQUIRE(Tracker::alive == 0);
        REQUIRE(Tracker::default_constructed == 0);
    }

    SECTION("Growing never default-constructs spare capacity")
    {
        {
            kxanz::vector<Tracker> v;
            for (int i = 0; i < 10; ++i) { v.push_back(Tracker{ i }); }
            REQUIRE(Tracker::default_constructed == 0);
            REQUIRE(Tracker::alive == 10);  // only the 10 live elements, not all 27 slots
            REQUIRE(v.at(9).id == 9);
        }
        REQUIRE(Tracker::alive == 0);
    }

    SECTION("pop_back destroys exactly the removed element")
    {
        kxanz::vector<Tracker> v;
        v.push_back(Tracker{ 1 });
        v.push_back(Tracker{ 2 });
        int destroyed_before = Tracker::destroyed;
        v.pop_back();
        REQUIRE(Tracker::destroyed == destroyed_before + 1);
        REQUIRE(Tracker::alive == 1);
    }

    SECTION("shrink_to_fit keeps the same number of live elements")
    {
        kxanz::vector<Tracker> v;
        for (int i = 0; i < 4; ++i) { v.push_back(Tracker{ i }); }
        v.shrink_to_fit();
        REQUIRE(Tracker::alive == 4);
        REQUIRE(Tracker::default_constructed == 0);
        REQUIRE(v.at(3).id == 3);
    }

    SECTION("Destroying the vector destroys every remaining element")
    {
        {
            kxanz::vector<Tracker> v;
            for (int i = 0; i < 5; ++i) { v.push_back(Tracker{ i }); }
            v.pop_back();
        }
        REQUIRE(Tracker::alive == 0);
        REQUIRE(Tracker::destroyed == Tracker::value_constructed + Tracker::copied + Tracker::moved);
    }

    SECTION("push_back with a tracker object many times")
    {
        {
            kxanz::vector<Tracker> v;
            for (int i = 0; i < 1000; ++i) { v.push_back(Tracker{ i }); }
            REQUIRE(v.get_size() == 1000);
            REQUIRE(Tracker::alive == 1000);
            REQUIRE(Tracker::default_constructed == 0);
            REQUIRE(v.at(999).id == 999);
        }
        REQUIRE(Tracker::alive == 0);
    }
}
