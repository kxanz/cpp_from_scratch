#pragma once

#include <cstddef>
#include <mutex>

// Write your solution here
// C++26 using GCC 16.2
// Debug with std::cerr or std::clog.
// !!! IMPORTANT !!!
// 99% of headers are pre-compiled for you server-side.
// If your submission fails to compile due to a missing header, add it to your submission.

namespace kxanz
{
    struct ControlBlock
    {
        size_t count_{ };
        mutable std::mutex mutex_;
    };

    template <typename T>
    class SharedPointer
    {
    public:

        SharedPointer() { }
        SharedPointer(T* pointer)
        {
        }

        SharedPointer(const SharedPointer& other) noexcept
        {
        }

        SharedPointer& operator=(const SharedPointer& other) noexcept
        {

        }

        SharedPointer(SharedPointer&& other) noexcept
        {

        }

        SharedPointer& operator=(SharedPointer&& other) noexcept
        {

        }

        ~SharedPointer()
        {

        }

        void reset(T* pointer)
        {

        }


        size_t get_count() const
        {

        }

        T* operator->() const { }
        T& operator*() const { }
        operator bool() const noexcept { }

    private:
        T * underlying_ptr { nullptr };
    };

    template <typename T, typename... Args>
    SharedPointer<T> make_shared(Args&&... args)
    {
    }
}

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in string.h / uniqueptr.h / vector.h; "(new)" = first appearing here.
//
// Items used (by function):
//   ControlBlock
//       EC++ Item 14 (covered); EMC++ Item 16, 19 (new)
//       GOAL: one block per owned object, shared by every owner; holds the count and the mutex that guards it
//   SharedPointer (class)
//       EC++ Item 13, 14 (covered); EMC++ Item 19 (new)
//       GOAL: several SharedPointers can own the same object; the LAST one to go away deletes it
//   data members
//       EC++ Item 22 (covered); EMC++ Item 19 (new)
//       GOAL: the object pointer, plus a pointer to the ControlBlock that every owner shares
//   SharedPointer()
//       EC++ Item 4 (covered)
//       GOAL: owns nothing; get_count() is 0
//   SharedPointer(T* pointer)
//       EC++ Item 13, 15, 18 (covered); EMC++ Item 19 (new)
//       GOAL: take ownership of a raw pointer and start its count at 1
//   copy constructor
//       EC++ Item 5, 12, 14 (covered); EMC++ Item 16 (new)
//       GOAL: share ownership — both point at the same object and block, and the count goes up (under the lock)
//   copy assignment
//       EC++ Item 10, 11, 29 (covered); EMC++ Item 16 (new)
//       GOAL: let go of what *this owned (it may have been the last owner), then share other's; self-assignment safe
//   move constructor
//       EC++ Item 25 (covered); EMC++ Item 14, 17, 23 (covered)
//       GOAL: take over other's ownership without changing the count; other ends up owning nothing
//   move assignment
//       EC++ Item 10 (covered); EMC++ Item 14, 17, 23 (covered)
//       GOAL: let go of what *this owned, then take over other's; self-move safe
//   ~SharedPointer()
//       EC++ Item 8, 13 (covered); EMC++ Item 16 (new)
//       GOAL: drop this owner's share; delete the object and the block only when no owners are left
//   reset(T* pointer)
//       EC++ Item 13 (covered); EMC++ Item 19 (new)
//       GOAL: drop this owner's share, then own pointer instead (reset(nullptr) leaves it owning nothing)
//   get_count()
//       EC++ Item 3 (covered); EMC++ Item 16 (new)
//       GOAL: how many SharedPointers own the object right now; reading it is thread safe even though it's const
//   operator-> / operator*
//       EC++ Item 15 (covered)
//       GOAL: reach the owned object, same as with a raw pointer
//   operator bool
//       EC++ Item 15 (covered); EMC++ Item 7 (covered)
//       GOAL: true when it owns something (the problem asks for an implicit conversion; std::shared_ptr's is explicit)
//   make_shared<T>(args...) — an extra beyond the required SharedPointer interface
//       EMC++ Item 21, 25 (covered); EMC++ Item 24 (new)
//       GOAL: build a T from args, passed along exactly as given, and return a SharedPointer that owns it
//
// Optional, after everything passes:
//   make_shared single allocation
//       EMC++ Item 21 (covered)
//       GOAL: put the object and its count in ONE allocation instead of two
//   atomic count instead of a mutex
//       EMC++ Item 16 (new), 40 (new)
//       GOAL: the same thread safety with std::atomic, and why a lone counter doesn't need a full mutex
