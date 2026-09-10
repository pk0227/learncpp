#include <iostream>
#include <utility>
#include <cassert>

class IntList
{
    int m_list[10]{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

public:
    // C++20 standard approach: separate non-const and const overloads
    int& operator[](int index)
    {
        assert(index >= 0 && static_cast<std::size_t>(index) < std::size(m_list));
        return m_list[index];
    }

    const int& operator[](int index) const
    {
        assert(index >= 0 && static_cast<std::size_t>(index) < std::size(m_list));
        return m_list[index];
    }

    // Note: C++23 introduced "deducing this" (explicit object parameter) which allows
    // unifying both overloads into a single function template:
    // auto&& operator[](this auto&& self, int index) { ... }
};

int main()
{
    IntList list1{};
    std::cout << "value at index 5 : " << list1[5] << "\n";
    list1[5] = 1234;
    std::cout << "value at index 5 : " << list1[5] << "\n";

    const IntList list2{};
    std::cout << "value at index 6 : " << list2[6] << "\n";
    // list2[6] = 4321; // compile error: assignment of read-only location

    // Uncomment below to test assertion failure on invalid index:
    // list1[12] = 1122;

    return 0;
}