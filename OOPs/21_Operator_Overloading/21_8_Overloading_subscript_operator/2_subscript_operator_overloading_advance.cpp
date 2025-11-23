#include <iostream>
#include <utility>
#include <cassert>

class IntList
{
    int m_list[10]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

public:
    auto&& operator[](this auto&& self, int index)
    {
        assert(index >= 0 && static_cast<std::size_t>(index) < std::size(self.m_list));
        return self.m_list[index];
    }
};

int main()
{
    IntList list1{};
    std::cout << "value at index 5 : " << list1[5] << "\n";
    list1[5] = 1234;
    std::cout << "value at index 5 : " << list1[5] << "\n";


    const IntList list2{};
    std::cout << "value at index 6 : " << list2[6] << "\n";
    //list2[6] = 4321;                                            // error: assignment of read-only location
    std::cout << "value at index 6 : " << list2[6] << "\n";

    list1[12] = 1122;   // Invalid index
    return 0;
}