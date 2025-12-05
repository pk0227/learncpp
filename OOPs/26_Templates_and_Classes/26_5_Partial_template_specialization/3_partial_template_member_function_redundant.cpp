#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

template <typename T, int size>
class StaticArray_Base
{
    T m_array[size]{};
public:
    T* getArray() { return m_array; }
    int getSize() const { return size; }
    const T& operator[](int index) const { return m_array[index]; }
    T& operator[](int index) { return m_array[index]; }
    void print() const;
    // Don't forget a virtual destructor if you're going to use virtual function resolution
};

template <typename T, int size>
void StaticArray_Base<T, size>::print() const
{
   for(int i{}; i < size; i++)
        std::cout << m_array[i] << " ";
}

template <typename T, int size>
class StaticArray : public StaticArray_Base<T, size>
{
};

template <int size>
class StaticArray<char, size> : public StaticArray_Base<char, size>
{
public:
    void print() const;
};

template <int size>
void StaticArray<char, size>::print() const
{
   for(int i{}; i < size; i++)
        std::cout << this->operator[](i);

// note: The this-> prefix in the above line is needed.
// See https://stackoverflow.com/a/6592617 or https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members for more info on why.
}

int main()
{
    //constexpr int size{5};
    //StaticArray<int, size> sai{};
    
    StaticArray<int, 5> sai{};

    for(int i{}; i < sai.getSize(); i++)
        sai[i] = (i*10);

    sai.print();
    std::cout << "\n";

    constexpr std::string_view sv{"Hello World"};
    StaticArray<char, sv.size()> sac{};
    std::copy_n(sv.begin(), sv.size(), sac.getArray());

    sac.print();
    std::cout << "\n";

    return 0;
}