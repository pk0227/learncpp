#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <exception>

int main()
{
    std::vector<int> v{10, 20, 30, 40, 50};
    
    // no issue while using std::vector<int>::size() / std::size() / std::ssize() directly in std::cout statement. 
    std::cout << "std::vector<int>::size() : " << v.size() << "\n";
    std::cout << "std::size() : " << std::size(v) << "\n";
    std::cout << "std::ssize() : " << std::ssize(v) << "\n";
    
    // issue while assigning return value of std::vector<int>::size() to other integral types.
    //int ret1{v.size()};     // compilation error if compiled with -Werror flag
    int ret1{static_cast<int>(v.size())};     // static_cast is the solution.
    
    // issue while assigning return value of std::size() to other integral types.
    //int ret2{std::size(v)};     // compilation error if compiled with -Werror flag
    int ret2{static_cast<int>(std::size(v))};     // static_cast is the solution.
    
    // issue while assigning return value of std::ssize() to other integral types.
    //int ret3{std::ssize(v)};     // compilation error if compiled with -Werror flag
    int ret3{static_cast<int>(std::ssize(v))};     // static_cast is the solution.
    
    // recommended type for variables holding length of containers
    std::size_t len1{v.size()};
    std::size_t len2{std::size(v)};
    std::ptrdiff_t len3{std::ssize(v)};
    
    // best solution is auto.
    auto ret4{v.size()};
    auto ret5{std::size(v)};   
    auto ret6{std::ssize(v)};   
    
    // operator[] doesn't check array index boundaries
    std::cout << std::format("v[{}] : {}", 2, v[2]) << "\n";        // no issue with integral literals
    
    //int index{10};                // Not recommended for index variables 
    //constexpr int index{10};      // safe to use constexpr , as it implicitly converts to std::size_t, no narrow conversion.
    std::size_t index{10};          // recommended to use std::size_t , as it requires no narrow conversion.
    std::cout << std::format("v[{}] : {}", index, v[index]) << "\n";        // operator[] does no bounds checking
    try
    {
        std::cout << std::format("v[{}] : {}", index, v.at(index)) << "\n";     // at() does bounds checking & throws exception std::out_of_range
    }
    catch(const std::exception& e)
    {
        std::cerr << "ExceptionError : " << e.what() << "\n";
    }
    
    return 0;
}