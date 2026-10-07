#include "String/string.h"
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <utility>

// Catch2::Catch2WithMain supplies main() — no CATCH_CONFIG_MAIN needed.
//

TEST_CASE("kxanz::string basic functionality", "[string]")
{
    SECTION("Default construction")
    {
        kxanz::string s;
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);
    }

    SECTION("Construct from C string")
    {
        kxanz::string s{ "hello" };
        REQUIRE(s.size() == 5);
        REQUIRE(!s.empty());
        REQUIRE(s == kxanz::string{ "hello" });
    }

    SECTION("Copy construction deep-copies the buffer")
    {
        kxanz::string a{ "hello" };
        kxanz::string b{ a };
        b[0] = 'H';
        REQUIRE(a[0] == 'h');
        REQUIRE(b[0] == 'H');
    }

    SECTION("Move construction leaves source empty")
    {
        kxanz::string a{ "hello" };
        kxanz::string b{ std::move(a) };
        REQUIRE(b.size() == 5);
        REQUIRE(a.empty());
    }

    SECTION("Copy assignment is independent from the source")
    {
        kxanz::string c{ "hello" };
        kxanz::string e{ "world" };
        e = c;
        REQUIRE(e.size() == c.size());
        e[0] = 'H';
        REQUIRE(c[0] == 'h');
    }

    SECTION("Self-assignment doesn't corrupt the string")
    {
        kxanz::string f{ "selftest" };
        f = f;
        REQUIRE(f.size() == 8);
        REQUIRE(f[0] == 's');
    }

    SECTION("Move assignment takes the source's buffer")
    {
        kxanz::string g{ "target" };
        kxanz::string h{ "source" };
        g = std::move(h);
        REQUIRE(g.size() == 6);
        REQUIRE(g[0] == 's');
    }

    SECTION("length() matches size()")
    {
        kxanz::string s{ "Hellows" };
        REQUIRE(s.length() == 7);
    }

    SECTION("empty() false path")
    {
        kxanz::string s{ "Hellows" };
        REQUIRE_FALSE(s.empty());
    }
}

TEST_CASE("operator[] and at()", "[string]")
{
    SECTION("operator[] (non-const) unchecked read/write")
    {
        kxanz::string idx{ "abc" };
        REQUIRE(idx[1] == 'b');
        idx[0] = 'X';
        REQUIRE(idx[0] == 'X');
    }

    SECTION("operator[] (const) resolves to the const overload")
    {
        const kxanz::string idxConst{ "abc" };
        REQUIRE(idxConst[2] == 'c');
    }

    SECTION("at() in range, const and non-const")
    {
        kxanz::string luis{ "Luis" };
        const kxanz::string constLuis{ "Luis" };
        REQUIRE(luis.at(3) == 's');
        REQUIRE(constLuis.at(3) == 's');
    }

    SECTION("at() out of range throws std::out_of_range")
    {
        kxanz::string luis{ "Luis" };
        REQUIRE_THROWS_AS(luis.at(100), std::out_of_range);
    }
}

TEST_CASE("append, operator+=, clear", "[string]")
{
    SECTION("append combines two strings without modifying the argument")
    {
        kxanz::string a{ "Foo" };
        kxanz::string b{ "bar" };
        a.append(b);
        REQUIRE(a.size() == 6);
        REQUIRE(a[0] == 'F');
        REQUIRE(a[5] == 'r');
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 'b');
    }

    SECTION("self-append doubles the string")
    {
        kxanz::string self{ "ab" };
        self.append(self);
        REQUIRE(self.size() == 4);
    }

    SECTION("operator+= forwards to append")
    {
        kxanz::string plusEq{ "x" };
        plusEq += kxanz::string{ "yz" };
        REQUIRE(plusEq.size() == 3);
        REQUIRE(plusEq[0] == 'x');
        REQUIRE(plusEq[2] == 'z');
    }

    SECTION("clear resets to the empty state")
    {
        kxanz::string s{ "x" };
        s.clear();
        REQUIRE(s.size() == 0);
        REQUIRE(s.empty());
    }
}

TEST_CASE("operator+", "[string]")
{
    SECTION("concatenates without modifying either operand")
    {
        kxanz::string a{ "foo" };
        kxanz::string b{ "bar" };
        kxanz::string c = a + b;
        REQUIRE(c.size() == 6);
        REQUIRE(c[0] == 'f');
        REQUIRE(c[5] == 'r');
        REQUIRE(a.size() == 3);
        REQUIRE(b.size() == 3);
    }
}

TEST_CASE("operator==, operator!=, operator<<", "[string]")
{
    SECTION("equal contents compare equal")
    {
        REQUIRE(kxanz::string{ "abc" } == kxanz::string{ "abc" });
        REQUIRE_FALSE(kxanz::string{ "abc" } != kxanz::string{ "abc" });
    }

    SECTION("different contents, same length compare unequal")
    {
        REQUIRE(kxanz::string{ "abc" } != kxanz::string{ "abd" });
        REQUIRE_FALSE(kxanz::string{ "abc" } == kxanz::string{ "abd" });
    }

    SECTION("different lengths compare unequal")
    {
        REQUIRE(kxanz::string{ "abc" } != kxanz::string{ "ab" });
        REQUIRE_FALSE(kxanz::string{ "abc" } == kxanz::string{ "ab" });
    }

    SECTION("operator<< writes the characters to the stream")
    {
        std::ostringstream oss;
        oss << kxanz::string{ "hello" };
        REQUIRE(oss.str() == "hello");
    }
}
