#pragma once

#include <atomic>
#include <thread>

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


