#pragma once

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in string.h; "(new)" = first appearing here.
#include <utility>

namespace kxanz
{

// EMC++ Item 18 (new) 
template <typename T>
struct underlying_ptr_deleter
{
    void operator()(T * underlying_ptr) const
    { delete underlying_ptr; }
};

// EMC++ Item 18 (new)
template <typename T, typename underlying_ptr_deleter = underlying_ptr_deleter<T>>
class unique_ptr
{
public:
    unique_ptr() = default;

    explicit unique_ptr(T * pointer) : underlying_ptr (pointer) {}

    unique_ptr(const unique_ptr&) = delete;

    unique_ptr& operator=(const unique_ptr&) = delete; 

    unique_ptr(unique_ptr&& other) noexcept : underlying_ptr(other.underlying_ptr) 
        { other.underlying_ptr = nullptr; }

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        T * incoming { std::exchange(other.underlying_ptr, nullptr) };
        T * oldPtr { std::exchange(underlying_ptr, incoming) };
        underlying_ptr_deleter()(oldPtr);
        return *this;
    }

    ~unique_ptr()
        { underlying_ptr_deleter()(underlying_ptr); }

    T* release() noexcept 
    {
        T * old = underlying_ptr;
        underlying_ptr = nullptr;
        return old;
    }

    void reset(T* pointer = nullptr) noexcept
    {
        T * old = underlying_ptr;
        underlying_ptr = pointer;
        underlying_ptr_deleter()(old);
    }

    bool is_owning() const noexcept
        { return underlying_ptr != nullptr; }


    T& operator*() const noexcept 
        { return *underlying_ptr; }

    T* operator->() const noexcept 
        { return underlying_ptr; }

    explicit operator bool() const noexcept 
        { return is_owning(); }

    // EC++ Item 15 (new)
private:
    T * underlying_ptr { nullptr };
};
}

// Book item references (moved here from the members above):
//   Default constructor
//       EC++ Item 5 (covered) done
//
//   Converting constructor (from a raw pointer)
//       EC++ Item 13 (covered) done
//       EMC++ Item 18 (new) done
//       EMC++ Item 21 (new, deferred) done
//
//   Copy constructor
//       EC++ Item 14 (new) done
//       EC++ Item 6 (new) done
//       EMC++ Item 11 (new) done
//
//   Copy assignment operator
//       EC++ Item 14 (new) done
//       EC++ Item 11 (covered) done
//
//   Move constructor
//       EC++ Item 12 (covered) done
//       EMC++ Item 17 (covered) done
//       EMC++ Item 23, 25 (covered) done
//
//   Move assignment operator
//       EC++ Item 10, 11, 25 (covered) done
//       EMC++ Item 14, 29 (covered) done
//
//   Destructor
//       EC++ Item 8 (covered) done
//       EC++ Item 13 (covered) done
//       EMC++ Item 18 (new)
//
//   release() — ordinary member function, not a special member
//       EMC++ Item 18 (new)
//
//   reset() — ordinary member function
//       EMC++ Item 18 (new)
//       EC++ Item 16 (covered) done
//
//   is_owning() — ordinary member function
//       EC++ Item 3 (covered) done
//
//   operator* / operator-> — dereference / member-access operators
//       EC++ Item 3 (covered)
//       EMC++ Item 18 (new)
//
//   operator bool() — (explicit) conversion operator
//       EMC++ Item 18 (new)
