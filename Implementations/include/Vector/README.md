# kxanz::vector

A from-scratch reimplementation of `std::vector`'s core behavior in C++, built item-by-item following *Effective C++* (EC++) and *Effective Modern C++* (EMC++) by Scott Meyers.

## How It Works

The vector owns one contiguous heap block and tracks it with three members: `data_` (the start of the block), `size_` (how many elements are alive), and `capacity_` (how many elements the block can hold). The vector object itself is just those three members — 24 bytes on a 64-bit platform — while the elements live on the heap.

Memory and object lifetime are handled separately. The `Allocator` (default `std::allocator<T>`) hands out **raw** memory, and each element is then built in place with placement `new` and torn down with `std::destroy_at`. That is what lets slots past `size_` stay unconstructed, so types with no default constructor work, and why `pop_back` can destroy an element without freeing its slot.

`push_back` only reallocates when the vector is full (`size_ == capacity_`). Growth multiplies the capacity by 2 (0 becomes 1), so reallocations get rarer as the vector grows and `push_back` is amortized O(1) instead of O(n²) with "+1" growth. Growth and `shrink_to_fit` share one private `reallocate(new_capacity)` helper: allocate the new block, move each element over, destroy the old elements, free the old block, then repoint `data_`.

The class follows the Rule of Five. Copies are deep: the copy constructor and copy assignment allocate a new block and copy-construct every element, building the new block before freeing the old one so a failed allocation leaves `*this` untouched (EC++ Item 29). Moves steal `data_`, `size_`, and `capacity_` with `std::exchange` and leave the source empty, so no element is copied or moved. Move operations are `noexcept` (EMC++ Item 14), and both assignments are safe against self-assignment.

## Operations

| Method | Description |
|---|---|
| `vector()` | Empty vector with capacity 1 |
| `explicit vector(size_t capacity)` | Empty vector with room for `capacity` elements |
| `vector(n, value)` | `n` copies of `value` |
| `vector{a, b, c}` | Build from an initializer list |
| `v = {a, b, c}` | Replace the contents with a list; builds a temporary and reuses move assignment for the cleanup |
| Copy constructor / copy assignment | Deep copy into a new block; self-assignment safe |
| Move constructor / move assignment | Steal the source's block and leave it empty; `noexcept`, self-move safe |
| `~vector()` | Destroy every element, then free the block |
| `push_back(value)` | Append, doubling the capacity when full |
| `pop_back()` | Destroy the last element (no-op when empty) |
| `at(i)` | Bounds-checked access; throws `std::out_of_range` |
| `get_size()` / `get_capacity()` / `empty()` | Element count, slot count, whether size is 0 |
| `shrink_to_fit()` | Reallocate so capacity equals size (skipped if already equal) |
| `clear()` | Destroy every element; capacity is kept |
| `begin()` / `end()` | Forward iterators, so range-for works |

## Build & Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target vector_test
./build/tests/vector_/vector_test
```

Tests are written with GoogleTest (fetched automatically via CMake's `FetchContent`) and run under AddressSanitizer/UndefinedBehaviorSanitizer in Debug builds. Run a single test with `--gtest_filter=VectorTest.PushBackTest`. The 16 tests cover every constructor, copy and move (including that a move copies no elements), destruction counts via an `AllocCounter` type, growth (capacity 1, 2, 4, 8, 16), `shrink_to_fit`, `pop_back`, `at` bounds checking, a type with no default constructor, and `sizeof`.

## Notes

Educational implementation for practicing raw memory management, placement `new`, object lifetime, amortized growth, the Rule of Five, and move semantics.

Not yet implemented: `operator[]`, a non-const `at`, a public `reserve`, `const` iterators (`begin() const` / `cbegin`), postfix `++` on the iterator, and `emplace_back`. Growth always moves elements rather than using `std::move_if_noexcept`, and the allocator is default-constructed on each use rather than stored.
