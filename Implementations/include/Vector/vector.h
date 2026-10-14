#pragma once

#include <cstddef>

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in string.h / uniqueptr.h; "(new)" = first appearing here.

// Write your solution here
//C++26 using GCC 16.2
// Debug with std::cerr or std::clog.
// !!! IMPORTANT !!!
// 99% of headers are pre-compiled for you server-side.
// If your submission fails to compile due to a missing header, add it to your submission.

namespace kxanz
{

// vector — EC++ Item 41 (new)
template <typename Element>
class vector
{
public:
    // push_back() — EC++ Item 13, 16, 29 (covered); EMC++ Item 14, 23, 41 (covered)
    void push_back(Element element)
    {
        // Implement.
    }

    // at() — EC++ Item 3, 28 (covered)
    const Element& at(std::size_t index) const
    {
        // Implement.
    }

    // get_size() — EC++ Item 3 (covered); EMC++ Item 14 (covered)
    std::size_t get_size() const
    {
        // Implement.
    }

    // get_capacity() — EC++ Item 3 (covered); EMC++ Item 14 (covered)
    std::size_t get_capacity() const
    {
        // Implement
    }

    // shrink_to_fit() — EC++ Item 16, 29 (covered); EMC++ Item 14, 23 (covered)
    void shrink_to_fit()
    {
        // Implement
    }

    // pop_back() — EC++ Item 8 (covered); EMC++ Item 14 (covered)
    void pop_back()
    {
        // Implement
    }

    // data members — EC++ Item 22 (new)
private:
    // Data
};
}

// Not in the provided code — add these yourself:
//   Default constructor — EC++ Item 4, 5, 13 (covered)
//       GOAL: capacity starts at 1
//   Destructor — EC++ Item 8, 13, 16 (covered)
//       GOAL: destroy only the live elements, then free the memory
//   Copy constructor / copy assignment — EC++ Item 10, 11, 12, 14, 29 (covered); EMC++ Item 11 (covered)
//       GOAL: implement them, or = delete them
//   Move constructor / move assignment — EC++ Item 10, 25 (covered); EMC++ Item 14, 17, 23, 29 (covered)
