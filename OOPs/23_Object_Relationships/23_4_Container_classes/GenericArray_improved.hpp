#ifndef GENERICARRAY_IMPROVED_HPP
#define GENERICARRAY_IMPROVED_HPP

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

/*
    IMPROVEMENTS MADE:
    1. Fixed incomplete #include on line 7
    2. Changed int to std::size_t for lengths/indices (proper type for array sizes)
    3. Added bounds checking in operator[] (with assertions in debug, exceptions in release)
    4. Fixed insert() signature - should take const T& value, not int value
    5. Added insert at end functionality (allow index == m_length)
    6. Added edge case handling for empty arrays in remove()
    7. Added explicit noexcept specifications where appropriate
    8. Improved self-assignment check efficiency
    9. Added additional utility functions (empty(), at(), data())
    10. Better const-correctness
*/

template <typename T>
class GenericArray
{
    T* m_array{};
    std::size_t m_length{};
    
public:
    // Constructors
    GenericArray(std::size_t length = 0); 
    GenericArray(std::initializer_list<T>);
    GenericArray(const GenericArray&); 
    
    // Assignment operators
    GenericArray& operator=(const GenericArray&);
    GenericArray& operator=(std::initializer_list<T>); 

    // Destructor
    ~GenericArray() noexcept
    {
        delete[] m_array; // delete[] nullptr is safe, no need to check
    } 

    // Element access
    T& operator[](std::size_t index) 
    { 
        #ifdef NDEBUG
            // In release mode, throw exception for safety
            if (index >= m_length)
                throw std::out_of_range("Index out of range");
        #else
            // In debug mode, use assertion for performance
            assert(index < m_length && "Index out of range");
        #endif
        return m_array[index]; 
    }
    
    const T& operator[](std::size_t index) const 
    { 
        #ifdef NDEBUG
            if (index >= m_length)
                throw std::out_of_range("Index out of range");
        #else
            assert(index < m_length && "Index out of range");
        #endif
        return m_array[index]; 
    }

    // Bounds-checked access (always throws on out of range)
    T& at(std::size_t index)
    {
        if (index >= m_length)
            throw std::out_of_range("Index out of range");
        return m_array[index];
    }

    const T& at(std::size_t index) const
    {
        if (index >= m_length)
            throw std::out_of_range("Index out of range");
        return m_array[index];
    }

    // Modifiers
    void erase() noexcept
    {
        delete[] m_array;
        m_array = nullptr;
        m_length = 0;
    }

    void reallocate(std::size_t length)
    {
        erase();
        if(length > 0)
        {
            m_array = new T[length]{};
            m_length = length;
        }
    }

    void resize(std::size_t);
    
    // Insert value at index (shifts elements right)
    // Now accepts index == m_length for append
    bool insert(std::size_t index, const T& value);
    
    // Remove element at index (shifts elements left)
    bool remove(std::size_t index);

    // Capacity
    std::size_t getLength() const noexcept { return m_length; }
    std::size_t size() const noexcept { return m_length; } // STL-like name
    bool empty() const noexcept { return m_length == 0; }

    // Direct access to underlying array (use with caution)
    T* data() noexcept { return m_array; }
    const T* data() const noexcept { return m_array; }
};

// Constructor with length
template <typename T>
GenericArray<T>::GenericArray(std::size_t length) : m_length{length}
{ 
    if(m_length > 0)
    {
        m_array = new T[m_length]{};
    }
}

// Constructor with initializer list
template <typename T>
GenericArray<T>::GenericArray(std::initializer_list<T> list) : GenericArray(list.size())
{
    std::size_t index{0};
    for (const T& value : list)
    {
        m_array[index++] = value;   // copy
    }
}

// Copy constructor
template <typename T>
GenericArray<T>::GenericArray(const GenericArray& ref) : GenericArray(ref.m_length)
{
    if(ref.m_length > 0)
        std::copy_n(ref.m_array, ref.m_length, m_array);
}

// Copy assignment operator
template <typename T>
GenericArray<T>& GenericArray<T>::operator=(const GenericArray& ref)
{
    if(this != &ref)  // More idiomatic self-assignment check
    {
        // Copy-and-swap idiom would be better, but this works
        if(ref.m_length != m_length)
        {
            delete[] m_array;
            m_length = ref.m_length;
            m_array = (m_length > 0) ? new T[m_length]{} : nullptr;
        }
        
        if(m_length > 0)
            std::copy_n(ref.m_array, m_length, m_array);
    }

    return *this;
}

// Assignment from initializer list
template <typename T>
GenericArray<T>& GenericArray<T>::operator=(std::initializer_list<T> list)
{
    std::size_t new_length = list.size();
    
    if(new_length != m_length)
    {
        delete[] m_array;
        m_length = new_length;
        m_array = (m_length > 0) ? new T[m_length]{} : nullptr;
    }

    if(m_length > 0)
    {
        std::size_t index{0};
        for (const T& value : list)
        {
            m_array[index++] = value;
        }
    }

    return *this;
}

// Resize array
template <typename T>
void GenericArray<T>::resize(std::size_t length)
{
    if(m_length == length)
        return;

    if(length == 0)
    {
        erase();
        return;
    }

    T* new_array = new T[length]{};

    if(m_length > 0)
    {
        std::size_t elementsToCopy = (m_length < length) ? m_length : length;
        std::copy_n(m_array, elementsToCopy, new_array);
    }

    delete[] m_array;
    m_array = new_array;
    m_length = length;
}

// Insert element at index
template <typename T>
bool GenericArray<T>::insert(std::size_t index, const T& value)
{
    // Allow inserting at end (index == m_length) for append functionality
    if(index > m_length)  // Changed from >= to >
        return false;
    
    T* new_array = new T[m_length + 1]{};
    
    // Copy elements before insertion point
    if(index > 0)
        std::copy_n(m_array, index, new_array);
    
    // Insert new element
    new_array[index] = value;
    
    // Copy elements after insertion point
    if(index < m_length)
        std::copy_n(m_array + index, m_length - index, new_array + index + 1);

    delete[] m_array;
    m_array = new_array;
    ++m_length; 

    return true;
}

// Remove element at index
template <typename T>
bool GenericArray<T>::remove(std::size_t index)
{
    if(index >= m_length || m_length == 0)
        return false;
    
    // Special case: removing last element or array becomes empty
    if(m_length == 1)
    {
        erase();
        return true;
    }
    
    T* new_array = new T[m_length - 1]{};
    
    // Copy elements before removal point
    if(index > 0)
        std::copy_n(m_array, index, new_array);
    
    // Copy elements after removal point
    if(index < m_length - 1)
        std::copy_n(m_array + index + 1, m_length - index - 1, new_array + index);

    delete[] m_array;
    m_array = new_array;
    --m_length; 

    return true;
}

#endif
