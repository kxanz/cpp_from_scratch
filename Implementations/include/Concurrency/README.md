# Concurrency

Concurrency primitives and thread-safe data structures built from scratch in C++20, following *C++ Concurrency in Action* (Anthony Williams) and the concurrency part of *Operating Systems: Three Easy Pieces*.

## Roadmap

Ordered by the chapter of *C++ Concurrency in Action* each project needs.

| # | Project | Difficulty | Main chapters | Status |
|---|---|---|---|---|
| 1 | [SPSC Queue](./SPSCQueue) | Easy | CCiA 7.2.6 (Listing 7.14), 5.2–5.3, 2.1–2.3 | 🚧 In progress |
| 2 | Reader-Writer Lock | Hard | CCiA 3.3.2, 4.1; OSTEP 31.5 | Planned |
| 3 | Mutex | — | CCiA 5.2–5.3; OSTEP 28.6–28.13 | Planned |
| 4 | Write-supremacy concurrent data structure | — | CCiA 3.3.2, 6.1–6.2; OSTEP 31.5 | Planned |
| 5 | Lock-Free SPSC Queue | Medium | CCiA 7.1–7.3, 5.3, 8.2.2–8.2.4 | Planned |
| 6 | Thread Pool | — | CCiA 9.1, 4.2 | Planned |
| 7 | stop_token | — | CCiA 9.2, 4.3.4 | Planned |
| 8 | std::jthread | — | CCiA 2.1–2.3 (Listing 2.7), 9.2 | Planned |

## Build & Run

```bash
cd Implementations
cmake -B build
cmake --build build --target spsc_queue_test
./build/tests/spsc_queue_/spsc_queue_test
```
