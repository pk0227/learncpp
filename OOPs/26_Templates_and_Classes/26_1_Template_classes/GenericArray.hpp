#ifndef GENERICARRAY_HPP
#define GENERICARRAY_HPP

#include <iostream>
#include <initializer_list>
#include <algorithm>

template <typename T>
class GenericArray
{
    T* m_array{};
    int m_length{};
    
public:
    GenericArray(int length=0); 
    GenericArray(std::initializer_list<T>);
    GenericArray(const GenericArray&); 
    GenericArray& operator=(const GenericArray&);
    GenericArray& operator=(std::initializer_list<T>); 

    ~GenericArray() 
    {
        delete[] m_array; 
    } 

    T& operator[](int index) { return m_array[index]; }
    const T& operator[](int index) const { return m_array[index]; }

    void erase()
    {
        delete[] m_array;
        m_array = nullptr;
        m_length = 0;
    }

    void reallocate(int length)
    {
        erase();
        if(length > 0)
        {
            m_array = new T[static_cast<std::size_t>(length)]{};
            m_length = length;
        }
    }

    int getLength() const { return m_length; }
    
    void resize(int);
    
    bool insert(int, const T&);
    bool remove(int);
};

#endif