#include <iostream>
#include <ios>       // for std::boolalpha

template <typename T>
class Pair
{
    T m_first{};
    T m_second{};

public:
    // When we define a member function inside the class definition,
    // the template parameter declaration belonging to the class applies
    Pair(const T& a, const T& b) : m_first{a}, m_second{b}
    {
    }

    //bool isEqual(const Pair<T>& obj);
    bool isEqual(const Pair& obj);          // Injected class names : note the parameter has type Pair, not Pair<T>
                                            // CTAD doesn’t work with function parameters (as it is argument deduction, not parameter deduction).
                                            // However, using an injected class name as a function parameter is okay, as it is shorthand for the fully templated name, not a use of CTAD.
 
};

// When we define a member function outside the class definition,
// we need to resupply a template parameter declaration
template <typename T>
//bool Pair<T>::isEqual(const Pair<T>& obj)
bool Pair<T>::isEqual(const Pair& obj)      // Injected class names : note the parameter has type Pair, not Pair<T>
{
    return ((m_first == obj.m_first) && (m_second == obj.m_second));
}

int main()
{
    Pair p1{5, 6};
    std::cout << std::boolalpha << p1.isEqual({5,6}) <<"\n";
    std::cout << std::boolalpha << p1.isEqual(Pair{5,7}) <<"\n";

    using namespace std::literals;
    Pair p2{"Hello"sv, "World"sv};

    return 0;
}