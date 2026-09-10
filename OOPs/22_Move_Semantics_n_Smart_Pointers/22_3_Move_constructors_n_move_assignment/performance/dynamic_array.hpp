#include <cstddef>

#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

template <typename T>
class DynamicArray
{
    T* m_array{};
    int m_length{};
    void alloc(int length)
    {
        m_array = new T[static_cast<std::size_t>(length)]{};
        m_length = length;
    }
public:
    DynamicArray(int length)
    {
        alloc(length);
    }

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    //DynamicArray(const DynamicArray&);
    //DynamicArray& operator=(const DynamicArray&);

    DynamicArray(DynamicArray&&) noexcept;
    DynamicArray& operator=(DynamicArray&&) noexcept;

    ~DynamicArray()
    {
        delete[] m_array;
    }

    int getLength() const { return m_length; }
    T& operator[](int index) { return m_array[index]; }
    const T& operator[](int index) const { return m_array[index]; }
};
/*
template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& ref)
{
    alloc(ref.m_length);
    std::copy_n(ref.m_array, ref.m_length, m_array);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& ref)
{
    if(&ref != this)
    {
        delete[] m_array;
        alloc(ref.m_length);
        std::copy_n(ref.m_array, ref.m_length, m_array);
    }

    return *this;
}
*/

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& ref) noexcept : m_array{ref.m_array}, m_length{ref.m_length}
{
    ref.m_length = 0;
    ref.m_array = nullptr;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& ref) noexcept
{
    if(&ref != this)
    {
        delete[] m_array;
        m_length = ref.m_length;
        ref.m_length = 0;
        m_array = ref.m_array;
        ref.m_array = nullptr;
    }

    return *this;
}

#endif