#pragma once

#include <cstddef>
#include <limits>
#include <new>
#include <memory>

namespace kxanz
{

    struct ChunkHeader 
    {
        const static inline size_t HeaderMagicId = std::numeric_limits<size_t>::max();
        size_t Size{ HeaderMagicId };
        size_t Magic{ HeaderMagicId };
    };

    struct Chunk 
    {
        ChunkHeader Header;
        std::byte * Memory { nullptr };
    };

    class Allocator 
    {
    public:
        explicit Allocator(size_t capacity)
            : capacity_ { capacity }
            , chunks_ { std::make_unique<std::byte[]>(capacity) }
    {  }

        Allocator(size_t capacity, std::byte* allocated)
            : capacity_ { capacity }
            , chunks_ { allocated }
    {  }

        ~Allocator() = default;

        Allocator(const Allocator&) = delete;
        void operator=(const Allocator&) = delete;
        Allocator(Allocator&&) = delete;
        void operator=(const Allocator&&) = delete;
        
        /**
         * @brief 
         */
        std::byte * Allocate(size_t capacity, size_t align = sizeof(void*)) 
        {
        }
        
        /**
         *
         */
        void Deallocate(std::byte * bytes) 
        {
            if (!bytes) { return; }


        }
        
    private:
        /**
         *
         */
        std::byte * AllocateNextSlot(size_t capacity)
        {

        }

    size_t capacity_;
    size_t used { 0 };
    std::unique_ptr<std::byte[]> chunks_{ nullptr };
};
}

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in an earlier project; "(new)" = first appearing here.
//
// Items used (by function):
//   Allocator (class)
//       EC++ Item 13, 14 (covered); EMC++ Item 11 (covered)
//       GOAL: the allocator is the sole owner of its buffer; copying and moving it are not allowed
//   data members
//       EC++ Item 22 (covered)
//       GOAL: everything the allocator needs to track fits in 24 bytes or less
//   explicit Allocator(size_t capacity)
//       EC++ Item 13 (covered)
//       GOAL: get a buffer of capacity bytes that this allocator owns
//   Allocator(size_t capacity, std::byte* allocated)
//       EC++ Item 13 (covered)
//       GOAL: take ownership of memory someone else already got, same as if you had allocated it yourself
//   Allocate(size_t capacity, size_t align)
//       EC++ Item 49 (covered); EC++ Item 50, 51 (new)
//       GOAL: write a ChunkHeader, hand back the bytes right after it, and bump forward
//       GOAL: every header and every chunk you hand out starts at a properly aligned address
//       GOAL: throw std::bad_alloc when the request doesn't fit, and handle a request for 0 bytes sensibly
//   Deallocate(std::byte* bytes)
//       EC++ Item 51 (new)
//       GOAL: find the header just behind bytes, check it's really one of yours, then set Size and Magic to 0
//       GOAL: follow the usual conventions for deallocating memory that isn't really there
//
// Not in the provided code — add it yourself:
//   ~Allocator()
//       EC++ Item 8, 13, 16 (covered)
//       GOAL: the buffer is released exactly once, with the form that matches how it was obtained
//
// Optional, after everything passes:
//   Plug it into kxanz::vector
//       EC++ Item 52 (new)
//       GOAL: understand how placement new and allocator-provided memory fit together
//
// Resources (by section):
//   Bump allocation / free space
//       OSTEP - Chapter 17, Free-Space Management (section 17.2)
//   Alignment
//       cppreference - alignof operator
//       cppreference - std::align
//   Byte buffers
//       cppreference - std::byte
