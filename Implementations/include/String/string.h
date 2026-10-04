#pragma once

#include <cstddef>
#include <cstring>
#include <iterator>
#include <ostream>
#include <cstddef>
#include <algorithm>
#include <stdexcept>

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept

namespace kxanz
{
    class string
    {
    public:
        // EC++ Item 4: initialize every member via the init list, not the body.
        // EC++ Item 5: know what the compiler would silently write here for you,
        // then decide deliberately whether the default is actually correct.
        string() = default;

        // EC++ Item 4, 5, 13: acquire the resource (buffer) in the constructor.
        // Practical: decide what happens if cstr == nullptr (empty string vs.
        // undefined behavior) — write that check as `cstr == nullptr`
        // (EMC++ Item 8: prefer nullptr to 0/NULL), not `cstr == NULL`.
        // Practical: allocate strlen(cstr) + 1 bytes — the +1 is for the null
        // terminator c_str() promises; allocate with new[] (see ~string, Item 16).
        string(const char* cstr)
        {
            if (cstr == nullptr) { return; }
            std::size_t length { std::strlen(cstr) };
            size_ = length;  
            data_ = new char[length + 1];
            capacity_ = length + 1;
            std::memcpy(data_, cstr, length + 1);
        }

        // EC++ Item 4, 12: copy every member, and copy the pointee, not the
        // pointer — a fresh buffer, not a second owner of the same one.
        string(const string& other) 
        {  
            std::size_t length = other.size_;
            size_ = length;
            data_ = new char[length + 1];
            capacity_ = length + 1;
            std::memcpy(data_, other.data_, length + 1);

        }

        // EC++ Item 12: move ctor should leave `other` destructible and steal its buffer.
        // EMC++ Item 17: understand why declaring this suppresses other implicitly
        // generated special members you might still want.
        // EMC++ Item 23, 25: this is what std::move is for — take other's pointer,
        // null out other's, don't allocate or copy any bytes.
        // EMC++ Item 14, 29: must stay noexcept, or std::vector<string> will
        // silently fall back to copying on reallocation instead of moving.
        string(string&& other) noexcept 
        {
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        // EC++ Item 8: destructors must not let exceptions escape.
        // EC++ Item 16: free with delete[] — it must match the new[] used to
        // acquire data_, never plain delete.
        // (Not EC++ Item 7: no virtual here — this class isn't a polymorphic base.)
        ~string() { delete[] data_; }

        // EC++ Item 10, 11, 12: return *this, handle self-assignment, copy all parts.
        // EC++ Item 29: consider copy-and-swap for the strong exception guarantee —
        // build the copy, then swap() it into *this only once it fully succeeded.
        string& operator=(const string& other)
        {    
            string temp(other);
            swap(temp);
            return *this;
        }

        // EC++ Item 10, 11, 25: return *this; handle self-move-assignment;
        // implementing via swap() gets you both almost for free.
        // EMC++ Item 14, 29: must stay noexcept for the same reason as the move ctor.
        string& operator=(string&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // EC++ Item 3: const-correctness — these never modify observable state.
        std::size_t size() const { return size_; }

        std::size_t length() const { return size_; }

        std::size_t capacity() const { return capacity_; }

        bool empty() const { return (size_ == 0) ? true : false; }

        // EC++ Item 3, 28: const/non-const overload pair; a non-const reference
        // out is a "handle" to internals, but it's operator[]'s documented
        // contract, not an accidental leak.
        // EC++ Item 18: match std::string's convention — operator[] is UB
        // out-of-range, at() throws std::out_of_range. Keep that distinction.
        char& operator[](std::size_t index) { return data_[index]; }

        const char& operator[](std::size_t index) const { return data_[index]; }

        char& at(std::size_t index)
        {
            if (index >= size_) {
                throw std::out_of_range("Kxanz::string::at: index out of range");
            }
            return data_[index];
        }

        const char& at(std::size_t index) const { return data_[index]; }

        // EC++ Item 28: c_str()/data() intentionally expose the internal buffer —
        // document why that's an acceptable exception, unlike operator[]'s
        // internals being handed out incidentally elsewhere.
        // Practical: the buffer must be null-terminated at data_[size_] for this
        // to be safe to hand to C APIs.
        const char* c_str() const { return data_; }

        const char* data() const { return data_; }

        // EC++ Item 5, 12: modifies *this in place; copy the bytes, not just a pointer.
        // EC++ Item 16: if you must grow, allocate the new buffer with new[],
        // copy over, then delete[] the old one — same rule as the destructor.
        // EC++ Item 29: build the grown buffer fully before touching *this, so a
        // failed allocation leaves the original string untouched.
        // Practical: grow geometrically (e.g. capacity * 2), not by exactly what's
        // needed each time, or every append becomes an O(n) reallocation.
        // Practical: watch self-aliasing — `s.append(s)` must not read from a
        // buffer you've already freed/reallocated mid-copy.
        string& append(const string& other)
        {
            std::size_t oldSize = size_;
            std::size_t otherSize = other.size_;
            char* oldData = data_;
            const char* otherData = other.data_;

            std::size_t newSize = oldSize + otherSize;
            data_ = new char[newSize + 1];

            std::memcpy(data_, oldData, oldSize);
            std::memcpy(data_ + oldSize, otherData, otherSize);
            data_[newSize] = '\0';

            delete[] oldData;
            size_ = newSize;
            capacity_ = newSize + 1;

            return *this;
        }

        string& operator+=(const string& other)
        {
            return append(other);
        }

        void clear()
        {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        }

        // EC++ Item 25: non-throwing swap, member half of the idiom.
        void swap(string& other) noexcept
        {
            using std::swap;

            swap(data_, other.data_);
            swap(size_, other.size_);
            swap(capacity_, other.capacity_);
        }

    private:
        char* data_{ nullptr };
        std::size_t size_{ 0 };
        std::size_t capacity_{ 0 };
    };

    // EC++ Item 25: non-member swap so std::swap-style code and ADL find it.
    inline void swap(string& lhs, string& rhs) noexcept { lhs.swap(rhs); }

    // EC++ Item 21, 24: non-member so implicit conversions (e.g. from const
    // char*) apply symmetrically to both operands; return a new object by
    // value rather than trying to return a reference to one.
    // Practical, extending EMC++ Item 41's reasoning (pass by value when the
    // parameter is always copied and cheap to move): take lhs by value
    // (`operator+(string lhs, ...)`) and return it after `lhs += rhs` — an
    // rvalue lhs is moved in for free, an lvalue is copied exactly once either way.
    inline string operator+(const string& lhs, const string& rhs)
    {
    }

    // EC++ Item 20, 23: pass by reference-to-const; prefer non-member/non-friend.
    // Practical: implement != in terms of == rather than duplicating the logic
    // (or in C++20, you can define only operator== and a defaulted <=>/rewritten
    // candidates cover != for you — worth trying once == works).
    inline bool operator==(const string& lhs, const string& rhs)
    {
    }

    inline bool operator!=(const string& lhs, const string& rhs)
    {
    }

    // EC++ Item 20, 23: pass by reference-to-const; must be non-member since
    // the left-hand operand is std::ostream, not kxanz::string.
    inline std::ostream& operator<<(std::ostream& os, const string& s)
    {
    }
}
