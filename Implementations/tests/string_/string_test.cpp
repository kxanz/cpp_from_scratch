#include "String/string.h"
#include <ostream>

// Temporary: plain main() for easy lldb debugging while string.h is still
// in progress. Catch2's TEST_CASE/SECTION macros generate extra control
// flow that confuses line-based breakpoints and variable-scope lookups.
// The real Catch2 tests are preserved commented-out below — restore them
// (and re-add Catch2::Catch2WithMain in CMakeLists.txt) once size()/
// c_str()/operator== are implemented and the REQUIREs can be uncommented.

int main()
{
    // Default construction
    kxanz::string a;

    // Construct from C string
    kxanz::string b{ "hello" };

    // Copy construction — should deep-copy the buffer
    kxanz::string c{ b };

    // Move construction — should leave the source empty
    kxanz::string d{ std::move(b) };

    return 0;
}

/*
#include <catch2/catch_test_macros.hpp>

// Catch2::Catch2WithMain supplies main() — no CATCH_CONFIG_MAIN needed.
// REQUIRE lines stay commented until size()/c_str()/operator== are
// implemented — calling them now is UB (no return statement yet).

TEST_CASE("kxanz::string basic functionality", "[string]")
{
    SECTION("Default construction")
    {
        kxanz::string s;
        // REQUIRE(s.empty());
        // REQUIRE(s.size() == 0);
    }

    SECTION("Construct from C string")
    {
        kxanz::string s{ "hello" };
        // REQUIRE(s.size() == 5);
        // REQUIRE(s == kxanz::string{ "hello" });
    }

    SECTION("Copy construction deep-copies the buffer")
    {
        // kxanz::string a{ "hello" };
        // kxanz::string b{ a };
        // b[0] = 'H';
        // REQUIRE(a[0] == 'h');
        // REQUIRE(b[0] == 'H');
    }

    SECTION("Move construction leaves source empty")
    {
        // kxanz::string a{ "hello" };
        // kxanz::string b{ std::move(a) };
        // REQUIRE(b.size() == 5);
        // REQUIRE(a.empty());
    }
}
*/
