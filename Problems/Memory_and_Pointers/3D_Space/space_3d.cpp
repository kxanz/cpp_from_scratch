#include <cstddef>
#include <limits>
#include <algorithm>
#include <iostream>
#include <new>
int*** GetSpace(size_t x, size_t y, size_t z) {
    if (!x || !y || !z) 
    {
        return nullptr;
    }

    int*** space = new int**[z];

    for (size_t i{}; i < z; ++i) 
    {
        auto** yAxis = space[i] = new int*[y];

        for (size_t j{}; j < y; ++j) 
        {
            auto* xAxis = yAxis[j] = new int[x];
            std::fill_n(xAxis, x, std::numeric_limits<int>::min());
        }
    }

    return space;
}

int main()
{
    size_t x {4}, y {5}, z {6};
    int*** space = GetSpace(x, y, z);

    if (space)
    {
        std::cout << "space[0][0][0] = " << space[0][0][0] << '\n';
        std::cout << "space[" << z - 1 << "][" << y - 1 << "][" << x - 1 << "] = "
                  << space[z - 1][y - 1][x - 1] << '\n';

        for (size_t i{}; i < z; ++i)
        {
            for (size_t j{}; j < y; ++j)
            {
                delete[] space[i][j];
            }
            delete[] space[i];
        }
        delete[] space;
    }

    std::cout << "GetSpace(0, 5, 6) returns " << (GetSpace(0, 5, 6) ? "a space" : "nullptr") << '\n';

    try
    {
        int*** huge = GetSpace(std::numeric_limits<size_t>::max(), 2, 2);
        std::cout << "huge input returns " << (huge ? "a space" : "nullptr") << '\n';
    }
    catch (const std::bad_alloc& e)
    {
        std::cout << "huge input threw " << e.what() << '\n';
    }
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
