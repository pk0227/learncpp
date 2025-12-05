#include <iostream>


template <typename T>
class Storage8
{
    T m_array[8]{};
public:
    void set(int index, const T& ref)
    {
        m_array[index] = ref;
    }
    const T& get(int index) const
    {
        return m_array[index];
    }
};

int main()
{
    Storage8<int> si{};
    for(int count{0}; count < 8; ++count)
        si.set(count, count);

    for(int count{0}; count<8; count++)
    {
        std::cout << std::boolalpha << si.get(count) << " ";
    }
    std::cout << "\n";

    Storage8<bool> sb{};
    for(int count{0}; count < 8; ++count)
        sb.set(count, count & 3);

    for(int count{0}; count<8; count++)
        std::cout << std::boolalpha << sb.get(count) << " ";
    std::cout << "\n";

    return 0;
}