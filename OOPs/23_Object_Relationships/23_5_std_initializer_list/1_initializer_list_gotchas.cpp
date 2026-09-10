#include <iostream>
#include <initializer_list>
#include <vector>
#include <cassert>

// Custom class demonstrating list constructor vs direct constructor matching
class SimpleArray
{
    int* m_data{};
    int m_size{};

public:
    // Non-list constructor: allocates array of given size
    explicit SimpleArray(int size)
        : m_data{ size > 0 ? new int[size]{} : nullptr }
        , m_size{ size > 0 ? size : 0 }
    {
        std::cout << "Called SimpleArray(int size) -> allocated size " << m_size << '\n';
    }

    // List constructor: initializes array with element values
    SimpleArray(std::initializer_list<int> list)
        : m_data{ list.size() > 0 ? new int[list.size()]{} : nullptr }
        , m_size{ static_cast<int>(list.size()) }
    {
        std::cout << "Called SimpleArray(initializer_list) -> element count " << m_size << '\n';
        int index{ 0 };
        for (int val : list) // Range-based for over initializer_list
        {
            m_data[index++] = val;
        }
    }

    // List assignment operator
    SimpleArray& operator=(std::initializer_list<int> list)
    {
        std::cout << "Called operator=(initializer_list) -> new count " << list.size() << '\n';
        delete[] m_data;
        m_size = static_cast<int>(list.size());
        m_data = m_size > 0 ? new int[m_size]{} : nullptr;

        int index{ 0 };
        // Alternative: accessing via begin() iterator
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            m_data[index++] = *it;
        }
        return *this;
    }

    ~SimpleArray()
    {
        delete[] m_data;
    }

    int getSize() const { return m_size; }

    int& operator[](int index) { return m_data[index]; }
    const int& operator[](int index) const { return m_data[index]; }
};

int main()
{
    std::cout << "=== 1. Direct Initialization vs List Initialization ===\n";
    // Direct initialization: calls SimpleArray(int)
    SimpleArray arr1(5);
    std::cout << "arr1 size: " << arr1.getSize() << "\n\n";

    // List initialization: always favors SimpleArray(initializer_list<int>)!
    SimpleArray arr2{ 5 };
    std::cout << "arr2 size: " << arr2.getSize() << ", arr2[0]: " << arr2[0] << "\n\n";

    // Multi-element list initialization
    SimpleArray arr3{ 10, 20, 30, 40 };
    std::cout << "arr3 size: " << arr3.getSize() << "\n\n";

    std::cout << "=== 2. std::vector Constructor Comparison ===\n";
    // std::vector demonstrates this exact same behavior:
    std::vector<int> v1(5); // 5 elements, all value-initialized to 0
    std::cout << "std::vector(5) length: " << v1.size() << " (all zeros)\n";

    std::vector<int> v2{ 5 }; // 1 element with value 5
    std::cout << "std::vector{5} length: " << v2.size() << " (element is " << v2[0] << ")\n\n";

    std::cout << "=== 3. List Assignment Operator ===\n";
    arr1 = { 100, 200, 300 };
    std::cout << "arr1 size after list assignment: " << arr1.getSize() << "\n";
    for (int i = 0; i < arr1.getSize(); ++i)
    {
        std::cout << arr1[i] << " ";
    }
    std::cout << "\n\n";

    std::cout << "=== 4. Iterating over std::initializer_list directly ===\n";
    auto ilist = { 1, 2, 3, 4, 5 };
    // initializer_list does NOT support operator[]: ilist[0] would be a compile error!
    // Must use iterators or range-based for:
    std::cout << "Direct iterator access: first=" << *ilist.begin() 
              << ", third=" << *(ilist.begin() + 2) << '\n';

    return 0;
}
