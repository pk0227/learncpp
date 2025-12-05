#include <iostream>

template <typename T>
class Storage
{
    T m_value{};
public:
    Storage(T val) : m_value{val}
    { }

    void print()
    {
        std::cout << m_value << "\n";
    }
};

// This double specialization is actually not needed for specializing member function.
// Explicit class template specialization for Storage<double>
// Note how redundant this is
/*
template <>
class Storage<double>
{
    double m_value{};
public:
    Storage(double val) : m_value{val}
    { }

    void print();
};
*/

// We're going to define this outside the class for reasons that will become obvious shortly
// This is a normal (non-specialized) member function definition (for member function print of specialized class Storage<double>)
template<>  // Adding this empty template is enough for specialing member function, in case, we don't specialize double template class
void Storage<double>::print()
{
    std::cout << std::scientific << m_value << "\n";
}

int main()
{
    // Define some storage units
    Storage i{5};
    Storage d{6.7};     // uses explicit specialization Storage<double>

    // Print out some values
    i.print();      // calls Storage<int>::print (instantiated from Storage<T>)
    d.print();      // calls Storage<double>::print (called from explicit specialization of Storage<double>)

    return 0;
}