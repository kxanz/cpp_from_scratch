#include <cstddef>

int*** GetSpace(size_t x, size_t y, size_t z) {
    // Implement
    return nullptr;
}

int main()
{
}

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
//
// Items used (by function):
//   GetSpace()
//       EC++ Item 16 (covered)
//       GOAL: every new[] you write is matched by the same form of delete[] when the space is freed
//       EC++ Item 49 (new)
//       GOAL: know what happens when an allocation fails, and leave nothing half-allocated behind
//       EC++ Item 13 (covered)
//       GOAL: notice which raw allocations a caller must remember to free, and in what order
