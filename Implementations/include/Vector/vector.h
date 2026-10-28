#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <iostream>
#include <memory>
#include <new>
#include <stdexcept>

// Write your solution here
//C++26 using GCC 16.2
// Debug with std::cerr or std::clog.
// !!! IMPORTANT !!!
// 99% of headers are pre-compiled for you server-side.
// If your submission fails to compile due to a missing header, add it to your submission.

namespace kxanz
{
template <typename T, typename Allocator = std::allocator<T>>
class vector
{
public:
      
    struct Iterator
    {
    public:
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::forward_iterator_tag;
      using pointer = T*;
      using reference = T&;

        Iterator(pointer ptr) : underlying_ptr(ptr) {}
        reference operator*() { return *underlying_ptr; }
        pointer operator->() { return underlying_ptr; }
        Iterator& operator++() { underlying_ptr++; return *this; }
        bool operator==(const Iterator& other) const { return underlying_ptr == other.underlying_ptr; }
       
    private:
        T* underlying_ptr;
    };
    
    // Alias
    using size_type = std::size_t;
    
    //Constructors
    /**  
    *  @brief creates a vector with no elements.
    */
    vector() : vector(1) {};


    /**
     * @param capacity reserve space on the vecotr
     */
    explicit vector(std::size_t capacity) { reserve(capacity); }

    /**
     * @param n Number of elements to initially create
     * @param init An element to copy
     */
    vector(size_type n, const T& init)
        : size_ { n }
        , capacity_ { n }
    {
        fill_construct(init);
    }

    vector(std::initializer_list<T> init)
    { 
        
    }

    vector& operator=(std::initializer_list<T> init);

    //Copy Constructor
    /**
    *   
    */
    explicit vector(const vector& other) = delete;

    vector& operator=(const vector& other) = delete;

    vector(vector&& other) noexcept;

    vector& operator=(vector&& other) noexcept;

    ~vector() 
    {
        clear();
        Allocator allocator;
        allocator.deallocate(data_, capacity_);
    }    

    void push_back(T data)
    {

    }
    
    /**
     * @brief returns value in the current index 
     */
    const T& at(size_type index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("index is out of range");
        }
        return data_[index];
    }
    
    /**
     * @brief returns the number of elements in the vector
     */
    [[nodiscard]] size_type get_size() const noexcept { return size_; }

    [[nodiscard]] size_type get_capacity() const noexcept { return capacity_; }

    void shrink_to_fit()
    {
    }

    void pop_back()
    {
    }
    
    [[nodiscard]] bool empty() const noexcept { return (size_ == 0) ? true : false; }

    void clear() { while (size_) { pop_back(); } }

    //void clear() const noexcept { while(size_) pop_back(); }

    Iterator begin() { return Iterator(data_); }
    Iterator end() { return Iterator(data_ + size_); }

private:
    /**
     * @brief Reserves raw memory for capacity_ elements, then constructs size_ 
     * copies of init into it.
     * @param init the value we allocating in the space.
     * */
    void fill_construct(const T& init)
    {
        Allocator allocator;
        data_ = allocator.allocate(capacity_);

        for (size_type i {0}; i < size_; ++i)
        {
            new (data_ + i) T(init);
        }
    }

    void reserve(size_type new_capacity)
    {
        if (new_capacity == 0)
        {
            return;
        }
        Allocator allocator;
        data_ = allocator.allocate(new_capacity);
        capacity_ = new_capacity;
    }

    void allocate_data(const std::initializer_list<T>& init)
    {
        reserve(init); 

        for (auto it = init.begin(); it != init.end(); it++)
        {
             
        }
    }

    

private:
    T * data_ { nullptr };
    size_type size_ {};
    size_type capacity_ {};

};
}

// Comment key:
//   EC++  = Effective C++, 3rd ed. (Meyers)      — pre-C++11, design/RAII/assignment
//   EMC++ = Effective Modern C++ (Meyers)         — C++11/14, move semantics, noexcept
// "(covered)" = an item already used in string.h / uniqueptr.h; "(new)" = first appearing here.
//
// Items used (by function, in the provided code above):
//   vector (class)    — EC++ Item 41 (new)
//   Iterator          — EC++ Item 47 (new); EMC++ Item 13 (new)
//   push_back()       — EC++ Item 13, 16, 29 (covered); EMC++ Item 14, 23, 41 (covered)
//   at()              — EC++ Item 3, 28 (covered)
//   get_size()        — EC++ Item 3 (covered); EMC++ Item 14 (covered)
//   get_capacity()    — EC++ Item 3 (covered); EMC++ Item 14 (covered)
//   shrink_to_fit()   — EC++ Item 16, 29 (covered); EMC++ Item 14, 23 (covered)
//   pop_back()        — EC++ Item 8 (covered); EMC++ Item 14 (covered)
//   data members      — EC++ Item 22 (new)

// Not in the provided code — add these yourself:
//   Default constructor — EC++ Item 4, 5, 13 (covered)
//       GOAL: capacity starts at 1
//   Destructor — EC++ Item 8, 13, 16 (covered)
//       GOAL: destroy only the live elements, then free the memory
//   Copy constructor / copy assignment — EC++ Item 10, 11, 12, 14, 29 (covered); EMC++ Item 11 (covered)
//       GOAL: implement them, or = delete them
//   Move constructor / move assignment — EC++ Item 10, 25 (covered); EMC++ Item 14, 17, 23, 29 (covered)

// Resources (by section):
//   Data members / layout
//       Microsoft - Inside STL: The vector
//       LearnCpp 16.1 - Introduction to containers and arrays
//   Constructor / initializer list
//       LearnCpp 16.2 - Introduction to std::vector and list constructors
//   get_size() / get_capacity() / empty()
//       LearnCpp 16.10 - std::vector resizing and capacity
//       LearnCpp 16.3 - std::vector and the unsigned length and subscript problem
//   at()
//       LearnCpp 16.3 (above)
//       LearnCpp 16.6 - Arrays and loops
//       LearnCpp 16.7 - Arrays, loops, and sign challenge solutions
//       LearnCpp 16.9 - Array indexing and length using enumerators
//   push_back() / growth
//       LearnCpp 16.11 - std::vector and stack behavior
//       LearnCpp 16.10 (above)
//       Vector Pessimization (Arthur O'Dwyer)
//   pop_back()
//       LearnCpp 16.11 (above)
//   shrink_to_fit()
//       cppreference - shrink_to_fit
//       LearnCpp 16.10 (above)
//   Move constructor / move assignment
//       LearnCpp 16.5 - Returning std::vector, and an introduction to move semantics
//       Vector Pessimization (above)
//       Microsoft - Inside STL: The vector (above)
//   General
//       LearnCpp 16.4 - Passing std::vector
//       LearnCpp 16.8 - Range-based for loops (for-each)
//       LearnCpp 16.12 - std::vector<bool>
