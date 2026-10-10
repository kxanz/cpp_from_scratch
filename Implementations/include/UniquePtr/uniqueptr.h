#pragma once

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in string.h; "(new)" = first appearing here.

#include <stdexcept>
namespace kxanz
{
    // EMC++ Item 18 (new) 
    template <typename T>
    struct underlying_ptr_deleter
    {
        void operator()(T * underlying_ptr) const
        {
            delete underlying_ptr;
        }
    };

    // EMC++ Item 18 (new)
    template <typename T, typename underlying_ptr_deleter = underlying_ptr_deleter<T>>
    class unique_ptr
    {
    public:
        // Default constructor
        // EC++ Item 5 (covered) done
        unique_ptr() = default;

        // Converting constructor (from a raw pointer)
        // EC++ Item 13 (covered) done
        // EMC++ Item 18 (new) done
        // EMC++ Item 21 (new, deferred) done
        unique_ptr(T * pointer) : underlying_ptr (pointer) {}

        // Copy constructor
        // EC++ Item 14 (new) done
        // EC++ Item 6 (new) done
        // EMC++ Item 11 (new)
        unique_ptr(const unique_ptr&)
            { throw std::logic_error("copy construction is not allowed"); }

        // Copy assignment operator
        // EC++ Item 14 (new) done
        // EC++ Item 11 (covered) done
        unique_ptr& operator=(const unique_ptr&) 
            { throw std::logic_error("copy assignment is not allowed"); }

        // Move constructor
        // EC++ Item 12 (covered) done
        // EMC++ Item 17 (covered) done
        // EMC++ Item 23, 25 (covered) done
        unique_ptr(unique_ptr&& other) noexcept
        {

        }

        // Move assignment operator
        // EC++ Item 10, 11, 25 (covered) done
        // EMC++ Item 14, 29 (covered) done
        unique_ptr& operator=(unique_ptr&& other) noexcept
        {

        }

        // Destructor
        // EC++ Item 8 (covered) done
        // EC++ Item 13 (covered) done
        // EMC++ Item 18 (new) 
        ~unique_ptr()
            { underlying_ptr_deleter()(underlying_ptr); }

        // release() — ordinary member function, not a special member
        // EMC++ Item 18 (new)
        T* release()
        {

        }

        // reset() — ordinary member function
        // EMC++ Item 18 (new)
        // EC++ Item 16 (covered)
        void reset(T* pointer)
        {

        }

        // is_owning() — ordinary member function
        // EC++ Item 3 (covered)
        bool is_owning() const { }


        // operator* / operator-> — dereference / member-access operators
        // EC++ Item 3 (covered)
        // EMC++ Item 18 (new)
        T& operator*() const { }
        T* operator->() const { }

        // operator bool() — (explicit) conversion operator
        // EMC++ Item 18 (new)
        operator bool() const { }

        // EC++ Item 15 (new)
    private:
        T * underlying_ptr { nullptr };
    };
}
