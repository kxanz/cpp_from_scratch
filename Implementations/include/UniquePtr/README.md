# kxanz::unique_ptr

A from-scratch reimplementation of `std::unique_ptr`'s core behavior in C++, built item-by-item following *Effective C++* (EC++) and *Effective Modern C++* (EMC++) by Scott Meyers.

## How It Works

The class holds a single raw pointer (`underlying_ptr`) and is its sole owner: when the `unique_ptr` is destroyed, reset, or move-assigned over, it frees the object it owned. Every free goes through the `Deleter` template parameter (default `underlying_ptr_deleter<T>`, which calls `delete`), never a hardcoded `delete`, so a custom deleter is respected everywhere (EMC++ Item 18).

Ownership can only be transferred, never shared. Copy construction and copy assignment are `= delete`, so an accidental copy is a compile error instead of a runtime failure (EMC++ Item 11). Move operations hand the pointer over and leave the source holding `nullptr`, so the moved-from object stays safely destructible. All move operations are `noexcept` (EMC++ Item 14).

Move assignment empties the source *first* with `std::exchange`, then swaps the incoming pointer into `*this` and frees the old one. Because the source is emptied before the old pointer is read, self-move-assignment (`p = std::move(p)`) is safe without an explicit `this != &other` check.

Both the raw-pointer constructor and `operator bool` are `explicit`. That blocks silent conversions such as `use(&x)` (a raw pointer quietly becoming an owner), `int n = p;`, and `p == q` comparing two `bool`s, while `if (p)`, `!p`, and `p && q` still work.

## Operations

| Method | Description |
|---|---|
| `unique_ptr()` | Default-construct an empty (non-owning) pointer |
| `explicit unique_ptr(T*)` | Take ownership of a raw pointer |
| Copy constructor / copy assignment | `= delete` — ownership can't be copied |
| Move constructor | Take the source's pointer and leave the source empty |
| Move assignment | Free the currently owned object, take the source's pointer, leave the source empty; self-move safe |
| `~unique_ptr()` | Free the owned object through the deleter (no-op when empty) |
| `release()` | Give up ownership *without* freeing; returns the raw pointer, which the caller now owns |
| `reset(p = nullptr)` | Free the currently owned object and take ownership of `p`; `reset()` just empties |
| `is_owning()` | Whether a non-null pointer is held |
| `operator*` / `operator->` | Access the owned object (UB if empty, same as `std::unique_ptr`) |
| `explicit operator bool` | Same as `is_owning()`, usable only in boolean contexts |

## Build & Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target uptr_test
./build/tests/uniqueptr_/uptr_test
```

Tests are written with Catch2 (fetched automatically via CMake's `FetchContent`) and run under AddressSanitizer/UndefinedBehaviorSanitizer in Debug builds. They cover ownership and destruction counts, every move case (into/from empty, self-move), `release`/`reset`, the access operators, and a counting custom deleter; `static_assert`s at the top pin down the compile-time guarantees (non-copyable, `noexcept` moves, `explicit` conversions).

## Notes

Educational implementation for practicing RAII, exclusive ownership, move semantics, `noexcept`, deleted functions, and explicit conversions — each piece verified against real compilation and sanitizer output rather than by reading the code alone.

Not yet implemented: `get()` (raw access without releasing ownership, EC++ Item 15). The deleter is default-constructed on each use rather than stored, so only stateless deleters are supported; there's no array (`T[]`) specialization and no converting moves between `unique_ptr<Derived>` and `unique_ptr<Base>`.
