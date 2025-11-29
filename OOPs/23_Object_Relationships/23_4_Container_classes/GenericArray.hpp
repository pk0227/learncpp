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

template <typename T>
GenericArray<T>::GenericArray(int length) : m_length{(length>0) ? length : 0}
{ 
    if(m_length)
    {
        m_array = new T[static_cast<std::size_t>(m_length)]{};
    }
}

template <typename T>
GenericArray<T>::GenericArray(std::initializer_list<T> list) : GenericArray(list.size())
{
/*
    for(int i{}; i<list.size(); i++)
    {
        m_array[i] = std::move(list.begin()[i]);    // move is NOT performed because std::initializer_list elements are const. So, it does not move anything — it becomes a copy.
    }
*/
    int index{};
    for (const T& value : list)
    {
        m_array[index++] = value;   // copy
    }
}

template <typename T>
GenericArray<T>::GenericArray(const GenericArray& ref) : GenericArray(ref.m_length)
{
    if(ref.m_length)
        std::copy_n(ref.m_array, ref.m_length, m_array);
}

template <typename T>
GenericArray<T>& GenericArray<T>::operator=(const GenericArray& ref)
{
    if(&ref != this)
    {
        erase();
        if(ref.m_length)
        {
            m_array = new T[static_cast<std::size_t>(ref.m_length)]{};
            std::copy_n(ref.m_array, ref.m_length, m_array);
            m_length = ref.m_length;
        }
    }

    return *this;
}

template <typename T>
GenericArray<T>& GenericArray<T>::operator=(std::initializer_list<T> list)
{
    erase();
    m_length = list.size();

    if(m_length)
    {
        m_array = new T[static_cast<std::size_t>(m_length)]{};
            
        int index{};
        for (const T& value : list)
        {
            m_array[index++] = value;   // copy
        }
    }

    return *this;
}

template <typename T>
void GenericArray<T>::resize(int length)
{
    if(m_length == length)
        return;

    if(length <= 0)
    {
        erase();
        return;
    }

    T* new_array{ new T[static_cast<std::size_t>(length)]{}};

    if(m_length)
    {
        int elementsToCopy{(m_length > length) ? length : m_length};
        std::copy_n(m_array, elementsToCopy, new_array);
    }

    delete[] m_array;
    m_array = new_array;
    m_length = length;
}

template <typename T>
bool GenericArray<T>::insert(int index, const T& value)
{
    if(index < 0 || index >= m_length)
        return false;
    
    T* new_array{ new T[static_cast<std::size_t>(m_length+1)]{}};
    
    std::copy_n(m_array, index, new_array);
    new_array[index] = value;
    std::copy_n(m_array+index, m_length-index, new_array+index+1);

    delete[] m_array;
    m_array = new_array;
    ++m_length; 

    return true;
}

template <typename T>
bool GenericArray<T>::remove(int index)
{
    if(index < 0 || index >= m_length)
        return false;
    
    // ✅ Handle single element case
    if(m_length == 1)
    {
        erase();
        return true;
    }

    T* new_array{ new T[static_cast<std::size_t>(m_length-1)]{}};
    
    std::copy_n(m_array, index, new_array);
    std::copy_n(m_array+index+1, m_length-index-1, new_array+index);

    delete[] m_array;
    m_array = new_array;
    --m_length; 

    return true;
}


#endif