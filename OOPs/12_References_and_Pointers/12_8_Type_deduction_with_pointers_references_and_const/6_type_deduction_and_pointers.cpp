#include <iostream>
#include <string>

std::string* getPtr() // some function that returns a pointer
{
    static std::string str{"Hello, World!"};
    return &str;
}

int main()
{
    auto ptr1{ getPtr() }; // std::string*
    auto* ptr2{ getPtr() }; // std::string*
    auto ptr3{ *getPtr() };      // std::string (because we dereferenced getPtr())
    auto* ptr4{ *getPtr() };     // does not compile (initializer not a pointer)

    std::cout << "ptr1 points to: " << *ptr1 << "\n";
    std::cout << "ptr2 points to: " << *ptr2 << "\n";
    std::cout << "ptr3 value: " << ptr3 << "\n";
    // std::cout << "ptr4 value: " << ptr4 << "\n"; // does not compile

    return 0;
}