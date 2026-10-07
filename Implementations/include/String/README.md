# kxanz::string

A from-scratch reimplementation of `std::string`'s core behavior in C++, built item-by-item following *Effective C++* (EC++) and *Effective Modern C++* (EMC++) by Scott Meyers.

## How It Works

The class owns a single heap-allocated, null-terminated `char` buffer (`data_`), tracked alongside `size_` (current length) and `capacity_` (allocated buffer size, including the null terminator). Every constructor and mutating operation that needs more room allocates a fresh buffer with `new[]`, copies bytes in with `memcpy`, and frees the old buffer with the matching `delete[]` — the class never mixes `new`/`new[]` with the wrong form of `delete` (EC++ Item 16).

Copy operations deep-copy the buffer (two `kxanz::string`s never share ownership of the same memory); move operations steal the source's pointer and null it out, leaving the moved-from object safely destructible. Copy assignment and move assignment are both implemented via a single `swap()` member, following the copy-and-swap idiom for strong exception safety (EC++ Item 29).

## Operations

| Method | Description |
|---|---|
| `string()` | Default-construct an empty string |
| `string(const char*)` | Construct from a null-terminated C string |
| Copy/move constructors, copy/move assignment | Rule of Five — deep-copy on copy, steal-and-null on move |
| `size()` / `length()` | Current number of characters |
| `capacity()` | Allocated buffer size (including the null terminator) |
| `empty()` | Whether the string holds zero characters |
| `operator[](i)` | Unchecked character access (UB if `i` is out of range) — const and non-const overloads |
| `at(i)` | Bounds-checked character access — throws `std::out_of_range` if `i >= size()` |
| `c_str()` / `data()` | Pointer to the internal null-terminated buffer |
| `append(other)` / `operator+=` | Grow the string in place by appending another `kxanz::string`'s characters; safe against self-append (`s.append(s)`) |
| `clear()` | Reset to the empty state, freeing the buffer |
| `swap(other)` | Non-throwing member swap; a non-member `swap()` free function delegates to it for ADL |
| `operator+` | Non-member; returns a new concatenated string without modifying either operand |
| `operator==` / `operator!=` | Non-member; compare length first, then raw bytes via `memcmp` |
| `operator<<` | Non-member; writes the string's characters to an `std::ostream` |

## Build & Run

```bash
cmake -B build
cmake --build build --target string_test
./build/tests/string_/string_test
```

Tests are written with Catch2 (fetched automatically via CMake's `FetchContent`) and run under AddressSanitizer/UndefinedBehaviorSanitizer by default in Debug builds.

## Notes

Educational implementation for practicing RAII, the Rule of Five, exception-safe assignment via copy-and-swap, const-correctness, and the `operator[]`-vs-`at()` contract — each piece verified against real compilation, lldb, and sanitizer output rather than by reading the code alone.
