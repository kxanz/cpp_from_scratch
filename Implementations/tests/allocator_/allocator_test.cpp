#include "Allocator/allocator.h"
#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <new>

// GTest::gtest_main supplies main() — no main() needed here.
//
// Run one test:    GTEST_FILTER=AllocatorTest.AllocateTest cmakerun allocator_test
// Run them all:    cmakerun allocator_test

namespace
{
    kxanz::ChunkHeader* HeaderOf(std::byte* bytes)
    {
        return reinterpret_cast<kxanz::ChunkHeader*>(bytes - sizeof(kxanz::ChunkHeader));
    }

    bool IsAligned(const void* address, size_t align)
    {
        return reinterpret_cast<std::uintptr_t>(address) % align == 0;
    }
}

TEST(AllocatorTest, DeallocateTest)
{
    kxanz::Allocator allocator(256);
    std::byte* first = allocator.Allocate(8);
    std::byte* second = allocator.Allocate(8);
    std::byte* third = allocator.Allocate(8);
    std::memcpy(third, "third!!", 8);

    EXPECT_NO_THROW(allocator.Deallocate(second));
    EXPECT_EQ(HeaderOf(second)->Size, 0u);
    EXPECT_EQ(HeaderOf(second)->Magic, 0u);

    EXPECT_EQ(HeaderOf(first)->Magic, kxanz::ChunkHeader::HeaderMagicId);
    EXPECT_EQ(HeaderOf(third)->Magic, kxanz::ChunkHeader::HeaderMagicId);

    std::byte* fourth = allocator.Allocate(8);
    EXPECT_NE(fourth, third);
    EXPECT_STREQ(reinterpret_cast<char*>(third), "third!!");

    EXPECT_NO_THROW(allocator.Deallocate(first));
    EXPECT_NO_THROW(allocator.Deallocate(third));
    EXPECT_NO_THROW(allocator.Deallocate(fourth));
}

TEST(AllocatorTest, DeallocateUnknownTest)
{
    kxanz::Allocator allocator(256);
    allocator.Allocate(8);

    alignas(16) std::byte elsewhere[64]{};
    EXPECT_THROW(allocator.Deallocate(elsewhere + 32), std::bad_alloc);

    std::byte* block = allocator.Allocate(8);
    allocator.Deallocate(block);
    EXPECT_THROW(allocator.Deallocate(block), std::bad_alloc);
}

TEST(AllocatorTest, AllocateTest)
{
    kxanz::Allocator allocator(256);
    std::byte* first = allocator.Allocate(8);
    std::byte* second = allocator.Allocate(8);

    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_EQ(second, first + 8 + sizeof(kxanz::ChunkHeader));

    EXPECT_EQ(HeaderOf(first)->Size, 8u);
    EXPECT_EQ(HeaderOf(second)->Size, 8u);
}

TEST(AllocatorTest, AllocateTooBigTest)
{
    kxanz::Allocator allocator(64);
    EXPECT_THROW(allocator.Allocate(64), std::bad_alloc);
    EXPECT_THROW(allocator.Allocate(std::numeric_limits<size_t>::max()), std::bad_alloc);

    EXPECT_NO_THROW(allocator.Allocate(64 - sizeof(kxanz::ChunkHeader)));
    EXPECT_THROW(allocator.Allocate(1), std::bad_alloc);
}

TEST(AllocatorTest, SizeTest)
{
    EXPECT_LE(sizeof(kxanz::Allocator), 24u);
}

TEST(AllocatorTest, AllocateEmptyTest)
{
    kxanz::Allocator allocator(64);
    std::byte* first = nullptr;
    std::byte* second = nullptr;

    EXPECT_NO_THROW(first = allocator.Allocate(0));
    EXPECT_NO_THROW(second = allocator.Allocate(0));

    EXPECT_NE(first, nullptr);
    EXPECT_NE(second, nullptr);
    EXPECT_NE(first, second);
    EXPECT_EQ(HeaderOf(first)->Size, 0u);
}

TEST(AllocatorTest, DeallocateEmptyTest)
{
    kxanz::Allocator allocator(64);
    EXPECT_NO_THROW(allocator.Deallocate(nullptr));

    std::byte* empty = allocator.Allocate(0);
    EXPECT_NO_THROW(allocator.Deallocate(empty));
}

TEST(AllocatorTest, CanYouReadInstructionsTest)
{
    kxanz::Allocator allocator(128);
    std::byte* block = allocator.Allocate(10);

    EXPECT_EQ(HeaderOf(block)->Size, 10u);
    EXPECT_EQ(HeaderOf(block)->Magic, kxanz::ChunkHeader::HeaderMagicId);

    allocator.Deallocate(block);
    EXPECT_EQ(HeaderOf(block)->Size, 0u);
    EXPECT_EQ(HeaderOf(block)->Magic, 0u);

    kxanz::Allocator owning(64, new std::byte[64]);
    std::byte* owned = owning.Allocate(8);
    EXPECT_NE(owned, nullptr);
}

TEST(AllocatorTest, AlignmentTest)
{
    kxanz::Allocator allocator(512);
    for (size_t request : { 3u, 5u, 1u, 7u, 13u })
    {
        std::byte* block = allocator.Allocate(request);
        EXPECT_TRUE(IsAligned(block, sizeof(void*)));
        EXPECT_TRUE(IsAligned(HeaderOf(block), alignof(kxanz::ChunkHeader)));
    }

    std::byte* sixteen = allocator.Allocate(4, 16);
    EXPECT_TRUE(IsAligned(sixteen, 16));

    std::byte* thirtyTwo = allocator.Allocate(4, 32);
    EXPECT_TRUE(IsAligned(thirtyTwo, 32));

    std::byte* zero = allocator.Allocate(4, 0);
    EXPECT_TRUE(IsAligned(HeaderOf(zero), alignof(kxanz::ChunkHeader)));
}
