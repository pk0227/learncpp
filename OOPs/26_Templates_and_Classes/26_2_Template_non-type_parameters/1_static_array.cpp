#include <iostream>

template <typename T, int size>
class StaticArray
{
    T m_array[size]{};
public:
    const T* getArray() const { return m_array; }

    const T& operator[](int index) const { return m_array[index]; }
    T& operator[](int index) { return m_array[index]; }
};

int main()
{
    StaticArray<int, 5> sa1{};
    StaticArray<int, 5> sa2{};
    StaticArray<int, 5> sa3{sa2};
    //StaticArray<int, 6> sa4{sa2};   // compilation error as size mismatches for copy constructor.
    StaticArray<int, 6> sa5{};

    sa2=sa1;
    //sa5=sa3;                        // compilation error as size mismatches for copy assignment operator.

    std::cout << sa1.getArray() << "\n";
    std::cout << sa2.getArray() << "\n";

    std::cout << "---------------------------------------------------\n";

    // declare an integer array with room for 12 integers
    StaticArray<int, 12> intArray;

    // Fill it up in order, then print it backwards
    for (int count { 0 }; count < 12; ++count)
        intArray[count] = count;

    for (int count { 11 }; count >= 0; --count)
        std::cout << intArray[count] << ' ';
    std::cout << '\n';

    // declare a double buffer with room for 4 doubles
    StaticArray<double, 4> doubleArray;

    for (int count { 0 }; count < 4; ++count)
        doubleArray[count] = 4.4 + 0.1 * count;

    for (int count { 0 }; count < 4; ++count)
        std::cout << doubleArray[count] << ' ';

    // int size{6};
    // StaticArray<double, size> doubleArray12;     // COMPILE ERROR: non-type template argument must be a constant expression
    constexpr int size{6};                          // Correct: constexpr value allows compiler to determine array size at compile time
    StaticArray<double, size> doubleArray12;

    return 0;
}