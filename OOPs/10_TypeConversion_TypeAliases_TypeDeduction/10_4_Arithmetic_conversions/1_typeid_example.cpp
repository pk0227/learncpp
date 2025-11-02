#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

int main()
{
    int i{ 2 };
    std::cout << typeid(i).name() << '\n'; // show us the name of the type for i

    double d{ 3.5 };
    std::cout << typeid(d).name() << '\n'; // show us the name of the type for d

    std::cout << typeid(i + d).name() << ' ' << i + d << '\n'; // show us the type of i + d

    std::cout << typeid(5u-10).name() << ' ' << 5u - 10 << '\n'; // 5u means treat 5 as an unsigned integer

    std::cout << std::boolalpha << (-3 < 5u) << '\n';

    // Demangle the type names for better readability
    int status;
    std::cout << abi::__cxa_demangle(typeid(i).name(), 0, 0, &status) << '\n';
    std::cout << abi::__cxa_demangle(typeid(d).name(), 0, 0, &status) << '\n';
    std::cout << abi::__cxa_demangle(typeid(i + d).name(), 0, 0, &status) << ' ' << i + d << '\n';
    std::cout << abi::__cxa_demangle(typeid(5u-10).name(), 0, 0, &status) << ' ' << 5u - 10 << '\n';
    
    return 0;
}