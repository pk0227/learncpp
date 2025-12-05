#include <iostream>
#include <cstdint>

// First define our non-specialized class template
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

// Now define our specialized class template
template <>     // the following is a template class with no templated parameters
class Storage8<bool>        // we're specializing Storage8 for bool
{
    // What follows is just standard class implementation details
    std::uint8_t m_data{};
public:
    // Don't worry about the details of the implementation of these functions
    bool get(int);
    void unset(int);
    void set(int, bool);
};

bool Storage8<bool>::get(int index)
{
    // Figure out which bit we're getting
    int mask{1 << index};
    // bitwise-and to get the value of the bit we're interested in
    // Then implicit cast to boolean
    return (m_data & mask);
}

void Storage8<bool>::unset(int index)
{
    int mask{1 << index};
    m_data &= ~mask;
}

void Storage8<bool>::set(int index, bool flag)
{
    // Figure out which bit we're setting/unsetting
    // This will put a 1 in the bit we're interested in turning on/off
    int mask{1 << index};
    if(flag)                // If we're setting a bit
        m_data |= mask;     // use bitwise-or to turn that bit on
    else                    // if we're turning a bit off
        unset(index);       // bitwise-and the inverse mask to turn that bit off
}

// Same example as before
int main()
{
    // Define a Storage8 for integers (instantiates Storage8<T>, where T = int)
    Storage8<int> si{};
    for(int count{0}; count < 8; ++count)
        si.set(count, count);

    for(int count{0}; count<8; count++)
    {
        std::cout << std::boolalpha << si.get(count) << " ";
    }
    std::cout << "\n";

    // Define a Storage8 for bool  (instantiates Storage8<bool> specialization)
    Storage8<bool> sb{};
    for(int count{0}; count < 8; ++count)
        sb.set(count, count & 3);

    for(int count{0}; count<8; count++)
        std::cout << std::boolalpha << sb.get(count) << " ";
    std::cout << "\n";

    return 0;
}