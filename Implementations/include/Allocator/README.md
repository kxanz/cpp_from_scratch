# kxanz::Allocator

A fixed-capacity **bump allocator** written from scratch in C++, based on the free-space management chapter of *Operating Systems: Three Easy Pieces* (OSTEP ch. 17.2) and the `new`/`delete` conventions in *Effective C++* (Items 49–52).

## How It Works

The allocator owns one buffer of raw bytes (`std::unique_ptr<std::byte[]>`). It either allocates the buffer itself or takes ownership of memory it's handed. It tracks three things: where the buffer starts (`chunks_`), how big it is (`capacity_`), and how many bytes are taken so far (`used_`). That's 24 bytes in total, and the buffer frees itself when the allocator is destroyed (Rule of Zero).

Every allocation is laid out the same way:

```
[ padding ][ ChunkHeader (16 bytes) ][ requested bytes ... ]
           ^                          ^
           start                      pointer returned to the client
```

The `ChunkHeader` stores the chunk's `Size` and a `Magic` number, so the allocator can later find and validate it from the client's pointer alone, the same way `free(ptr)` steps back to its header in OSTEP.

**Allocate** is O(1). There's no searching: the next chunk always goes right after the last one, at `used_`.

1. Raise `align` to at least `alignof(ChunkHeader)`, so the header is never misaligned (this also makes `align = 0` safe).
2. Work out the padding so that the **client's pointer** (16 bytes after the header) is a multiple of `align`.
3. Throw `std::bad_alloc` if the chunk doesn't fit. A request bigger than the whole buffer is rejected first, so the size arithmetic can't overflow.
4. Build the header in place with placement `new` and set its `Size`.
5. Move `used_` forward past the padding, header and requested bytes, then return the address right after the header.

**Deallocate** steps back `sizeof(ChunkHeader)` bytes from the client's pointer, `reinterpret_cast`s that address to a `ChunkHeader*`, and checks the `Magic` number. If it doesn't match, the pointer isn't a live chunk from this allocator, so it throws `std::bad_alloc`. That also catches double frees. Otherwise it sets `Size` and `Magic` to 0. Freed chunks in the middle of the buffer are never reused, which is the bump allocator's tradeoff: it's very fast, and the memory comes back when the whole allocator goes away. If the freed chunk is the **last** one, `used_` rolls back so its space can be reused, the way a stack allocator works.

## Operations

| Method | Description |
|---|---|
| `explicit Allocator(size_t capacity)` | Allocates and owns a buffer of `capacity` bytes |
| `Allocator(size_t capacity, std::byte* allocated)` | Takes ownership of an existing buffer of `capacity` bytes |
| Copy / move | Deleted: the allocator is the sole owner of its buffer |
| `Allocate(size_t n, size_t align = sizeof(void*))` | Returns `n` bytes aligned to `align`; throws `std::bad_alloc` if they don't fit; `Allocate(0)` returns a valid, distinct pointer |
| `Deallocate(std::byte* p)` | Marks the chunk dead (`Size` and `Magic` set to 0); no-op for `nullptr`; throws `std::bad_alloc` for pointers it doesn't manage |

## Build & Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target allocator_test
./build/tests/allocator_/allocator_test
```

Tests are written with GoogleTest and run under AddressSanitizer/UndefinedBehaviorSanitizer in Debug builds. Run a single test with `--gtest_filter=AllocatorTest.AllocateTest`. The 9 tests cover:
- freeing a block in the middle without disturbing its neighbors;
- rejecting unknown pointers and double frees;
- back-to-back placement of consecutive blocks;
- requests that are too big, including `SIZE_MAX` overflow;
- the 24-byte size limit;
- zero-byte allocations and `nullptr` frees;
- the header contents before and after a free;
- alignment for odd sizes and for `align` = 0, 16 and 32.

## Notes

Educational implementation for practicing raw memory, placement `new`, pointer arithmetic, alignment, and the `new`/`delete` conventions in EC++ Items 49–52.

Not yet implemented: reusing freed chunks in the middle of the buffer (a free list with splitting and coalescing, OSTEP 17.3+), and a range check in `Deallocate` so that a pointer from outside the buffer is rejected before its header is read.
