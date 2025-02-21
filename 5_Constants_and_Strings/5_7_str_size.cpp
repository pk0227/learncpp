#include <iostream>
#include <string>

int main()
{
    std::string name{ "Rama" };
    int length { static_cast<int>(name.length()) }; // std::string::length() returns an unsigned integral value (most likely of type size_t). 
                                                    // If you want to assign the length to an int variable, you should static_cast it 
                                                    // to avoid compiler warnings about signed/unsigned conversions.
    
    int len { static_cast<int>(std::ssize(name)) }; // In C++20, you can also use the std::ssize() function to get the length of a std::string 
                                                    // as a large signed integral type (usually std::ptrdiff_t).
                                                    // Since a ptrdiff_t may be larger than an int, if you want to store the result of std::ssize()
                                                    // in an int variable, you should static_cast the result to an int.

    //std::cout << name << " has " << name.length() << " characters\n";
    //std::cout << name << " has " << std::ssize(name) << " characters\n";
    
    std::cout << name << " has " << length << " characters\n";
    std::cout << name << " has " << len << " characters\n";

    return 0;
}