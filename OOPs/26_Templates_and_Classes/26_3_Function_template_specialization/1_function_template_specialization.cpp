#include <iostream>

// Here's our primary template (must come first)
template <typename T>
void print(const T& ref)
{
    std::cout << ref << "\n";
}

// A full specialization of primary template print<T> for type double
// Full specializations are not implicitly inline, so make this inline if put in header file
template<>                      // template parameter declaration containing no template parameters
void print(const double& ref)   // specialized for type double
{
    std::cout << std::scientific << ref << "\n";
}

int main()
{
    print(5);
    print(6.7);

    return 0;
}