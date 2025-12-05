#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

template <typename T, int size>
class StaticArray
{
    T m_array[size]{};
public:
    T* getArray() { return m_array; }
    int getSize() const { return size; }
    const T& operator[](int index) const { return m_array[index]; }
    T& operator[](int index) { return m_array[index]; }
};

template <typename T, int size>
void print(StaticArray<T, size>& ref)
{
   for(int i{}; i < ref.getSize(); i++)
        std::cout << ref[i] << " ";
}

template <int size>
void print(StaticArray<char, size>& ref)
{
   for(int i{}; i < ref.getSize(); i++)
        std::cout << ref[i];
}

int main()
{
    //constexpr int size{5};
    //StaticArray<int, size> sai{};
    
    StaticArray<int, 5> sai{};

    for(int i{}; i < sai.getSize(); i++)
        sai[i] = (i*10);

    print(sai);
    std::cout << "\n";

    constexpr std::string_view sv{"Hello World"};
    StaticArray<char, sv.size()> sac{};
    std::copy_n(sv.begin(), sv.size(), sac.getArray());

    print(sac);
    std::cout << "\n";

    return 0;
}