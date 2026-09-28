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

 
## Problems
 
### Operating Systems
 
| Problem |
|---|
| [Base and Bounds](./Problems/Operating_Systems/Base_and_Bounds) |
| [Adress Translation1](./Problems/Operating_Systems/Adress_Translation1) |

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
 
