#pragma once

#include <atomic>
#include <thread>

// Write your solution here
// C++26 using GCC 16.2
// Debug with std::cerr or std::clog.
// !!! IMPORTANT !!!
// 99% of headers are pre-compiled for you server-side.
// If your submission fails to compile due to a missing header, add it to your submission.

namespace kxanz
{
    template <typename T>
    struct DataWrapper
    {
        T data{ };
        bool is_last_chunk{ false };
    };

    template <typename T, typename Callback>
    class SPSC
    {
    public:
        SPSC(Callback callback)
        {
            // Implement
        }

        void PushWork(const DataWrapper<T>& wrapper)
        {
            // Implement
        }

    private:

        void Consume()
        {
            // Implement
        }
    };
}

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept, concurrency
//   CCiA  = C++ Concurrency in Action, 2nd ed. (Williams)
//   OSTEP = Operating Systems: Three Easy Pieces
// "(covered)" = an item already used in an earlier project; "(new)" = first appearing here.
//
// Items used (by function):
//   SPSC (class)
//       EC++ Item 13, 14 (covered); EMC++ Item 11 (covered)
//       GOAL: the queue owns every item still inside it; nobody can copy or move a queue
//             (the problem says "throw"; EMC++ 11 says the compiler should stop it — pick one and know why)
//   data members
//       EC++ Item 22 (covered); EMC++ Item 40 (new)
//       GOAL: an unbounded chain of items with one end for the producer and one for the consumer,
//             the callback, and the consumer thread
//       GOAL: think about which member has to be constructed last, and why
//   SPSC(Callback callback)
//       EC++ Item 13 (covered); EMC++ Item 37 (new)
//       GOAL: keep the callback and start the consumer thread spinning on Consume()
//   PushWork(const DataWrapper<T>& wrapper)
//       EMC++ Item 40 (new)
//       GOAL: add one item in O(1), touching only the producer's end of the queue
//       GOAL: the consumer must never see an item before it is fully built
//   Consume()
//       EMC++ Item 40 (new)
//       GOAL: spin until work shows up, hand each item to the callback in the order it was pushed, O(1) each
//       GOAL: stop for good after the item marked is_last_chunk
//
// Not in the provided code — add it yourself:
//   ~SPSC()
//       EC++ Item 8, 13 (covered); EMC++ Item 37 (new)
//       GOAL: the consumer thread is finished before the queue goes away, on every path
//       GOAL: every item still in the queue is freed exactly once
//
// Resources (by section):
//   The queue itself (unbounded, node based)
//       CCiA 7.2.6 — Listing 7.14 only (single-producer, single-consumer); the rest of 7.2.6 is multi-producer
//       CCiA 6.2.2 / 4.1.2 — the same queue with a mutex, to compare against
//   Making the producer's writes visible to the consumer
//       CCiA 5.1, 5.2.0–5.2.3 — atomics, atomic<bool>, atomic<T*>
//       CCiA 5.3.1–5.3.3 — synchronizes-with, happens-before, acquire/release
//   Starting and joining the consumer thread
//       CCiA 2.1, 2.2, 2.3
//   Why spinning costs CPU
//       OSTEP 28.12, 28.13; CCiA 8.2.2
//   Testing threaded code
//       CCiA 11.2.4, 11.2.5 (Listing 11.1)
