#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>
#include <memory>
#include <algorithm>

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
         * @b
         */
        std::byte * Allocate(size_t capacity, size_t align = sizeof(void*)) 
        {
            align = std::max(align, alignof(ChunkHeader));

            size_t remainder { (used_ + sizeof(ChunkHeader)) % align};
            size_t padding {};
            if (remainder)
            {
                padding = align - remainder;
            }
            size_t start { used_ + padding };

            if (start + sizeof(ChunkHeader) + capacity > capacity_ ||
                capacity > capacity_)
                throw std::bad_alloc();
            
            auto * allocation = AllocateNextSlot(capacity, start);
            const auto used = capacity + sizeof(ChunkHeader);

            if (allocation)
            {
                used_ += used + padding;
            }
            return allocation;
        }
        
        /**
         * @brief  
         */
        void Deallocate(std::byte * bytes) 
        {
            if (!bytes) { return; }
            
            ChunkHeader * header = reinterpret_cast<ChunkHeader*>(bytes - sizeof(ChunkHeader));
            std::byte * bytesEnd = header->Size + bytes;

            if (header->Magic != ChunkHeader::HeaderMagicId)
                throw std::bad_alloc();
            
            if (chunks_.get() + used_ == bytesEnd)
            {
                used_ -= header->Size + sizeof(ChunkHeader); 
            }
            header->Size = header->Magic = 0;
        }
        
    private:
        /**
         * 
         */
        std::byte * AllocateNextSlot(size_t capacity, size_t start)
        {
            if (!chunks_) return nullptr;
 
            std::byte * location = chunks_.get() + start;
            new (location) ChunkHeader{ capacity };

            return location + sizeof(ChunkHeader); 
        }

    size_t capacity_;
    size_t used_ { 0 };
    std::unique_ptr<std::byte[]> chunks_{ nullptr };
};
}


