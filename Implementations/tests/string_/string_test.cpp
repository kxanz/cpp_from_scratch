#include "String/string.h"
#include <cstddef>
#include <iostream>
#include <iterator>

// Temporary: plain main() for easy lldb debugging while string.h is still
// in progress. Catch2's TEST_CASE/SECTION macros generate extra control
// flow that confuses line-based breakpoints and variable-scope lookups.
// The real Catch2 tests are preserved commented-out below — restore them
// (and re-add Catch2::Catch2WithMain in CMakeLists.txt) once size()/
// c_str()/operator== are implemented and the REQUIREs can be uncommented.

int main()
{
    /*
    // Default construction
    kxanz::string a;

    // Construct from C string
    kxanz::string b{ "hello" };

    // Copy construction — should deep-copy the buffer
    kxanz::string c{ b };

    // Move construction — should leave the source empty
    kxanz::string d{ std::move(b) };

    // Copy assignment — e should end up independent from c
    kxanz::string e{ "world" };
    e = c;

    // Self-assignment — must not corrupt f
    kxanz::string f{ "selftest" };
    f = f;

    // Move assignment — g should take h's buffer; h ends up with g's old one
    kxanz::string g{ "target" };
    kxanz::string h{ "source" };
    g = std::move(h);

    // size() check — force a real call so lldb has something to inspect
    std::size_t aSize = a.size();
    std::size_t bSize = c.size();
    std::size_t cCapacity = c.capacity();

    std::cout << "size testing: " << aSize << '\n';
    std::cout << "size testing: " << bSize << '\n';
    
    kxanz::string s { "Hellows" };
    std::size_t sLength = s.length() ;
    std::cout << "length: " << sLength << '\n';

    std::cout << "capacity size: " << cCapacity << '\n';

    bool sIsEmpty = s.empty();   // s is "Hellows" — NOT empty, tests the false path
    std::cout << "s.empty(): " << std::boolalpha << sIsEmpty << '\n';

    kxanz::string Luis { "Luis" };
    std::cout << "luis at position: " << Luis.at(3) << '\n';

    // operator[] (non-const) — unchecked read and write through the reference
    kxanz::string idx { "abc" };
    char idxRead = idx[1];
    idx[0] = 'X';
    std::cout << "idx[1]: " << idxRead << ", idx[0] after idx[0]='X': " << idx[0] << '\n';

    // operator[] (const) — resolves to the const overload since idxConst is const
    const kxanz::string idxConst { "abc" };
    char idxConstRead = idxConst[2];
    std::cout << "idxConst[2]: " << idxConstRead << '\n';

    // at() (const) — same in-range call as Luis.at(3) above, but through a const object
    const kxanz::string constLuis { "Luis" };
    std::cout << "constLuis at position: " << constLuis.at(3) << '\n';

    // at() out-of-range — must throw std::out_of_range, not silently misbehave
    try
    {
        Luis.at(100);
        std::cout << "at(100) did NOT throw — bug!\n";
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "at(100) threw as expected: " << e.what() << '\n';
    }
    */
    
    kxanz::string a {"Foo"};
    kxanz::string b {"bar"};

    a.append(b);
    std::cout << "a after append(b): " << a.c_str() << " (size=" << a.size() << ")\n";
    std::cout << "b unchanged: " << b.c_str() << " (size=" << b.size() << ")\n";

    kxanz::string self {"ab"};
    self.append(self);
    std::cout << "self after self.append(self): " << self.c_str() << " (size=" << self.size() << ")\n";

    kxanz::string plusEq {"x"};
    plusEq += kxanz::string{"yz"};
    std::cout << "plusEq after += : " << plusEq.c_str() << " (size=" << plusEq.size() << ")\n";

    plusEq.clear();
    std::cout << "plusEq after clear(): size=" << plusEq.size() << ", empty=" << std::boolalpha << plusEq.empty() << '\n';

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
