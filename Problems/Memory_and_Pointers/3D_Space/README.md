# 3D Space

**Topic:** Memory and Pointers (Heap Allocation)

## Background

A 3D array on the heap can be reached through an `int***`: a pointer to an array of `int**`, where each of those points to an array of `int*`, and each of those points to a row of `int`s. Indexing it as `space[k][j][i]` follows three pointers to reach one `int`.

How those pointers are laid out in memory is up to you, and it matters: it decides how many allocations you make, how much can go wrong part-way through, and how hard the space is to free afterwards.

## Task

Implement `GetSpace`:

```cpp
int*** GetSpace(size_t x, size_t y, size_t z);
```

Allocates a 3D array of `int`s with dimensions `x`, `y`, and `z` on the heap and returns it. `z` is the outermost dimension, so an element is reached as `space[k][j][i]` with `k < z`, `j < y`, and `i < x`.

## Requirements

- Return an `int***`, with `z` as the outermost dimension.
- Don't use any data structures (such as `std::vector`).
- Initialize every value to `std::numeric_limits<int>::min()` during allocation **without a for-loop**. Prefer an STL algorithm over a raw loop.
- Consider edge cases fully. Return `nullptr` for invalid inputs.

## Example

Inputs are written as `x,y,z`. For example, `4,5,6` means `size_t x = 4, y = 5, z = 6;`.

```cpp
int*** space = GetSpace(4, 5, 6);
// space[0][0][0] ... space[5][4][3] are all std::numeric_limits<int>::min()
```

## Build & Run

```bash
clang++ -std=c++20 -Wall -Wextra space_3d.cpp -o space_3d && ./space_3d
```

`main` also tries a huge input. Under AddressSanitizer that allocation aborts the program with an "out-of-memory" report instead of throwing `std::bad_alloc`, so build without `-fsanitize` to see the normal behavior.
