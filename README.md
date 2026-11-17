# C++ From Scratch
 
Implementations of STL components, smart pointers, and coding problems — built in modern C++ to understand how things work under the hood.
 
Each folder contains its own README with design notes, complexity analysis, and things I learned along the way.
 
---
 
## STL & Core Components
 
| Implementation |
|---|
| [Hash Table](./Implementations/include/Hash) |
| [Unique Pointer](./Implementations/include/UniquePtr) |
| [String](./Implementations/include/String) |
| [Vector](./Implementations/include/Vector) |
| [Allocator](./Implementations/include/Allocator) (in progress) |

## Concurrency

Concurrency primitives and thread-safe data structures, following *C++ Concurrency in Action* (Anthony Williams) and the concurrency part of *Operating Systems: Three Easy Pieces*. Ordered by the chapter each project needs.

| # | Implementation | Difficulty | Main chapters | Status |
|---|---|---|---|---|
| 1 | [SPSC Queue](./Implementations/include/SPSCQueue) | Easy | CCiA 7.2.6 (Listing 7.14), 5.2–5.3, 2.1–2.3 | 🚧 In progress |
| 2 | Reader-Writer Lock | Hard | CCiA 3.3.2, 4.1; OSTEP 31.5 | Planned |
| 3 | Mutex | — | CCiA 5.2–5.3; OSTEP 28.6–28.13 | Planned |
| 4 | Write-supremacy concurrent data structure | — | CCiA 3.3.2, 6.1–6.2; OSTEP 31.5 | Planned |
| 5 | Lock-Free SPSC Queue | Medium | CCiA 7.1–7.3, 5.3, 8.2.2–8.2.4 | Planned |
| 6 | Thread Pool | — | CCiA 9.1, 4.2 | Planned |
| 7 | stop_token | — | CCiA 9.2, 4.3.4 | Planned |
| 8 | std::jthread | — | CCiA 2.1–2.3 (Listing 2.7), 9.2 | Planned |

## Problems
 
### Operating Systems
 
| Problem |
|---|
| [Base and Bounds](./Problems/Operating_Systems/Base_and_Bounds) |
| [Adress Translation1](./Problems/Operating_Systems/Adress_Translation1) |

### Memory and Pointers

| Problem |
|---|
| [3D Space](./Problems/Memory_and_Pointers/3D_Space) |

---
 
## Building
 
All implementations use C++20 and are built with CMake:

```bash
cd Implementations
cmake -B build
cmake --build build
```
 
## Structure
 
```
cpp_from_scratch/
├── Implementations/
│   ├── CMakeLists.txt
│   ├── include/       # Header implementations
│   └── tests/         # Tests per component
└── Problems/          # Coding problems
```
 
## Goals
 
- Understand the internals of the standard library, not just its API
- Practice manual memory management, RAII, and move semantics
- Write tested, readable modern C++
## Notes
 
These are educational implementations. They prioritize clarity over completeness and are not drop-in replacements for the standard library.
 
