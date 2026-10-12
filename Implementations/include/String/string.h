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
    // EC++ Item 4, 5 done
    string() = default;

    // EC++ Item 4, 5, 13 done
    // EMC++ Item 8 done
    string(const char* cstr)
    {
        if (cstr == nullptr) { return; }
        std::size_t length { std::strlen(cstr) };
        size_ = length;
        data_ = new char[length + 1];
        capacity_ = length + 1;
        std::memcpy(data_, cstr, length + 1);
    }

    // EC++ Item 4, 12 done
    string(const string& other)
    {
        std::size_t length = other.size_;
        size_ = length;
        data_ = new char[length + 1];
        capacity_ = length + 1;
        std::memcpy(data_, other.data_, length + 1);

    }

    // EC++ Item 12 done
    // EMC++ Item 17 done
    // EMC++ Item 23, 25 done
    // EMC++ Item 14, 29 done
    string(string&& other) noexcept
    {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // EC++ Item 8 done
    // EC++ Item 16 done
    ~string() { delete[] data_; }

    // EC++ Item 10, 11, 12 done
    // EC++ Item 29 done
    string& operator=(const string& other)
    {
        string temp(other);
        swap(temp);
        return *this;
    }

    // EC++ Item 10, 11, 25 done
    // EMC++ Item 14, 29 done
    string& operator=(string&& other) noexcept
    {
        swap(other);
        return *this;
    }

    // EC++ Item 3 done
    std::size_t size() const
        { return size_; }

    std::size_t length() const
        { return size_; }

    std::size_t capacity() const
        { return capacity_; }

    bool empty() const
        { return (size_ == 0) ? true : false; }

    // EC++ Item 3, 28 done
    // EC++ Item 18 done
    char& operator[](std::size_t index)
        { return data_[index]; }

    const char& operator[](std::size_t index) const
        { return data_[index]; }

    char& at(std::size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("Kxanz::string::at: index out of range");
        }
        return data_[index];
    }

    const char& at(std::size_t index) const
        { return data_[index]; }

    // EC++ Item 28
    const char* c_str() const
        { return data_; }

    const char* data() const
        { return data_; }

    // EC++ Item 5, 12 done
    // EC++ Item 16 done
    // EC++ Item 29 done
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
        { return append(other); }

    void clear()
    {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    // EC++ Item 25 done
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

// EC++ Item 25 done
inline void swap(string& lhs, string& rhs) noexcept
    { lhs.swap(rhs); }

// EC++ Item 21, 24 done
// EMC++ Item 41 done
inline string operator+(const string& lhs, const string& rhs)
{
    string result(lhs);
    result.append(rhs);
    return result;
}

// EC++ Item 20, 23 done
inline bool operator==(const string& lhs, const string& rhs)
{
    if (lhs.size() != rhs.size()) return false;
    return std::memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
}

inline bool operator!=(const string& lhs, const string& rhs)
    { return !(lhs == rhs); }

// EC++ Item 20, 23 done
inline std::ostream& operator<<(std::ostream& os, const string& s)
{
    os << s.c_str();
    return os;
}
}
