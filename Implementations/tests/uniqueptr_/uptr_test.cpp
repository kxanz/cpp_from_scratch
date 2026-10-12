#include "UniquePtr/uniqueptr.h"
#include <catch2/catch_test_macros.hpp>
#include <type_traits>
#include <utility>

// Catch2::Catch2WithMain supplies main() — no CATCH_CONFIG_MAIN needed.
//

// Counts live/destroyed instances so tests can prove exactly when deletes happen.
struct Test
{
    static inline int alive{ 0 };
    static inline int destroyed { 0 };
    int id{};

    explicit Test(int i = 0) : id{ i } { ++alive; }
    ~Test() { --alive; ++destroyed; }
};

// Custom deleter — proves the class routes every delete through its Deleter parameter.
struct CountingDeleter
{
    static inline int calls{ 0 };
    void operator()(Test * p) const { ++calls; delete p; }
};

// Catch2 re-runs the TEST_CASE body from the top for every SECTION,
// so resetting here gives each SECTION fresh counters.
static void reset_counters()
{
    Test::alive = 0;
    Test::destroyed = 0;
    CountingDeleter::calls = 0;
}

// Compile-time guarantees (EMC++ Item 11, Item 14; explicit conversions)
static_assert(!std::is_copy_constructible_v<kxanz::unique_ptr<Test>>);
static_assert(!std::is_copy_assignable_v<kxanz::unique_ptr<Test>>);
static_assert(std::is_nothrow_move_constructible_v<kxanz::unique_ptr<Test>>);
static_assert(std::is_nothrow_move_assignable_v<kxanz::unique_ptr<Test>>);
static_assert(!std::is_convertible_v<Test *, kxanz::unique_ptr<Test>>);  // explicit ctor
static_assert(std::is_constructible_v<kxanz::unique_ptr<Test>, Test *>);
static_assert(!std::is_convertible_v<kxanz::unique_ptr<Test>, bool>);    // explicit operator bool
static_assert(std::is_constructible_v<bool, kxanz::unique_ptr<Test>>);

TEST_CASE("kxanz::unique_ptr construction and destruction", "[unique_ptr]")
{
    reset_counters();

    SECTION("Default construction owns nothing")
    {
        kxanz::unique_ptr<Test> p;
        REQUIRE_FALSE(p.is_owning());
        REQUIRE_FALSE(p);
    }

    SECTION("Construct from raw pointer takes ownership")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 1 } };
        REQUIRE(p.is_owning());
        REQUIRE(p);
        REQUIRE(Test::alive == 1);
    }

    SECTION("Destructor deletes the owned object exactly once")
    {
        {
            kxanz::unique_ptr<Test> p{ new Test{ 1 } };
            REQUIRE(Test::alive == 1);
        }
        REQUIRE(Test::alive == 0);
        REQUIRE(Test::destroyed == 1);
    }

    SECTION("Destroying an empty unique_ptr deletes nothing")
    {
        {
            kxanz::unique_ptr<Test> p;
        }
        REQUIRE(Test::destroyed == 0);
    }
}

TEST_CASE("move construction and move assignment", "[unique_ptr]")
{
    reset_counters();

    SECTION("Move construction transfers ownership and empties the source")
    {
        kxanz::unique_ptr<Test> a{ new Test{ 1 } };
        kxanz::unique_ptr<Test> b{ std::move(a) };
        REQUIRE_FALSE(a.is_owning());
        REQUIRE(b.is_owning());
        REQUIRE(b->id == 1);
        REQUIRE(Test::alive == 1);
    }

    SECTION("Move assignment deletes the target's old object and takes the source's")
    {
        kxanz::unique_ptr<Test> a{ new Test{ 1 } };
        kxanz::unique_ptr<Test> b{ new Test{ 2 } };
        b = std::move(a);
        REQUIRE_FALSE(a.is_owning());
        REQUIRE(b->id == 1);
        REQUIRE(Test::alive == 1);
        REQUIRE(Test::destroyed == 1);
    }

    SECTION("Move assignment into an empty unique_ptr")
    {
        kxanz::unique_ptr<Test> a{ new Test{ 1 } };
        kxanz::unique_ptr<Test> b;
        b = std::move(a);
        REQUIRE(b->id == 1);
        REQUIRE(Test::destroyed == 0);
    }

    SECTION("Move assignment from an empty unique_ptr empties the target")
    {
        kxanz::unique_ptr<Test> a;
        kxanz::unique_ptr<Test> b{ new Test{ 2 } };
        b = std::move(a);
        REQUIRE_FALSE(b.is_owning());
        REQUIRE(Test::alive == 0);
        REQUIRE(Test::destroyed == 1);
    }

    SECTION("Self move-assignment keeps the object alive")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 7 } };
        auto & alias = p;  // goes through a reference to avoid -Wself-move
        p = std::move(alias);
        REQUIRE(p.is_owning());
        REQUIRE(p->id == 7);
        REQUIRE(Test::alive == 1);
        REQUIRE(Test::destroyed == 0);
    }

    SECTION("Move assignment returns *this")
    {
        kxanz::unique_ptr<Test> a{ new Test{ 1 } };
        kxanz::unique_ptr<Test> b;
        REQUIRE(&(b = std::move(a)) == &b);
        REQUIRE(b->id == 1);
    }
}

TEST_CASE("release() and reset()", "[unique_ptr]")
{
    reset_counters();

    SECTION("release() returns the pointer without deleting it")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 3 } };
        Test * raw = p.release();
        REQUIRE_FALSE(p.is_owning());
        REQUIRE(raw != nullptr);
        REQUIRE(raw->id == 3);
        REQUIRE(Test::alive == 1);
        delete raw;
    }

    SECTION("release() on an empty unique_ptr returns nullptr")
    {
        kxanz::unique_ptr<Test> p;
        REQUIRE(p.release() == nullptr);
    }

    SECTION("release() hands ownership to another unique_ptr")
    {
        kxanz::unique_ptr<Test> a{ new Test{ 4 } };
        kxanz::unique_ptr<Test> b{ a.release() };
        REQUIRE_FALSE(a.is_owning());
        REQUIRE(b->id == 4);
        REQUIRE(Test::alive == 1);
    }

    SECTION("reset(p) deletes the old object and takes p")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 1 } };
        p.reset(new Test{ 2 });
        REQUIRE(p->id == 2);
        REQUIRE(Test::alive == 1);
        REQUIRE(Test::destroyed == 1);
    }

    SECTION("reset() with no argument deletes and empties")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 1 } };
        p.reset();
        REQUIRE_FALSE(p.is_owning());
        REQUIRE(Test::alive == 0);
    }

    SECTION("reset() on an empty unique_ptr is harmless")
    {
        kxanz::unique_ptr<Test> p;
        p.reset();
        p.reset();
        REQUIRE_FALSE(p.is_owning());
        REQUIRE(Test::destroyed == 0);
    }

    SECTION("reset(p) on an empty unique_ptr takes p")
    {
        kxanz::unique_ptr<Test> p;
        p.reset(new Test{ 5 });
        REQUIRE(p->id == 5);
        REQUIRE(Test::destroyed == 0);
    }
}

TEST_CASE("operator*, operator->, operator bool", "[unique_ptr]")
{
    reset_counters();

    SECTION("operator* gives a reference to the owned object")
    {
        kxanz::unique_ptr<int> p{ new int{ 5 } };
        REQUIRE(*p == 5);
        *p = 6;
        REQUIRE(*p == 6);
    }

    SECTION("operator-> reaches the owned object's members")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 9 } };
        REQUIRE(p->id == 9);
        p->id = 10;
        REQUIRE((*p).id == 10);
    }

    SECTION("const unique_ptr still gives non-const access to the object")
    {
        const kxanz::unique_ptr<int> p{ new int{ 1 } };
        *p = 2;
        REQUIRE(*p == 2);
        REQUIRE(p.is_owning());
    }

    SECTION("operator bool works in boolean contexts")
    {
        kxanz::unique_ptr<Test> full{ new Test{ 1 } };
        kxanz::unique_ptr<Test> empty;
        REQUIRE(full);
        REQUIRE_FALSE(empty);
        REQUIRE(!empty);
        REQUIRE((full && !empty));
        REQUIRE(static_cast<bool>(full));
    }

    SECTION("operator bool tracks ownership changes")
    {
        kxanz::unique_ptr<Test> p{ new Test{ 1 } };
        REQUIRE(p);
        p.reset();
        REQUIRE_FALSE(p);
    }
}

TEST_CASE("custom deleter", "[unique_ptr]")
{
    reset_counters();

    SECTION("Destructor uses the custom deleter")
    {
        {
            kxanz::unique_ptr<Test, CountingDeleter> p{ new Test{ 1 } };
        }
        REQUIRE(CountingDeleter::calls == 1);
        REQUIRE(Test::alive == 0);
    }

    SECTION("reset() uses the custom deleter")
    {
        kxanz::unique_ptr<Test, CountingDeleter> p{ new Test{ 1 } };
        p.reset(new Test{ 2 });
        REQUIRE(CountingDeleter::calls == 1);
        REQUIRE(Test::alive == 1);
    }

    SECTION("Move assignment uses the custom deleter on the old object")
    {
        kxanz::unique_ptr<Test, CountingDeleter> a{ new Test{ 1 } };
        kxanz::unique_ptr<Test, CountingDeleter> b{ new Test{ 2 } };
        b = std::move(a);
        REQUIRE(CountingDeleter::calls == 1);
        REQUIRE(b->id == 1);
    }

    SECTION("release() never calls the deleter")
    {
        kxanz::unique_ptr<Test, CountingDeleter> p{ new Test{ 1 } };
        Test * raw = p.release();
        REQUIRE(CountingDeleter::calls == 0);
        delete raw;
    }
}
