#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

std::string* getPtr() // some function that returns a pointer
{
    static std::string str{"Hello, World!"};
    return &str;
}

int main()
{
    int status = 0;
    std::string str1{"main_string"};
    std::string str2{"another_string"};

    const auto ptr1{ getPtr() };  // std::string* const             // pointer is const
    auto const ptr2 { getPtr() }; // std::string* const             // pointer is const

    const auto* ptr3{ getPtr() }; // const std::string*              // pointer to const object
    auto* const ptr4{ getPtr() }; // std::string* const            // pointer is const

    //ptr1 = &str1; // error: ptr1 is const pointer
    //ptr2 = &str1; // error: ptr2 is const pointer
    //ptr4 = &str1; // error: ptr4 is const pointer

    std::cout << "before modification ptr3 points to: " << *ptr3 << "\n";
    ptr3 = &str1; // ok: ptr3 is pointer to const.
    std::cout << "after modification ptr3 points to: " << *ptr3 << "\n";

    /*
    std::cout << "typeid of ptr3 : " << typeid(ptr3).name() << " : ";
    std::cout << abi::__cxa_demangle(typeid(ptr3).name(), 0, 0, &status) << "\n";
    */

    std::cout << *getPtr() << "\n";

    std::cout << "--------------------------------------------------------------------------------\n";

    //ptr3->append(" Modified through ptr3.");      // error: cannot modify a const object.
    //*ptr3 = str2;                                 // error: cannot modify a const object.

    ptr1->append(" ptr1 ");      // ok: pointer is const, but object is not const.
    std::cout << "static string after modification through ptr1: " << *getPtr() << "\n";

    ptr2->append(" ptr2 ");      // ok: pointer is const, but object is not const.
    std::cout << "static string after modification through ptr2: " << *getPtr() << "\n";

    ptr4->append(" ptr4 ");      // ok: pointer is const, but object is not const.
    std::cout << "static string after modification through ptr4: " << *getPtr() << "\n";

    *ptr4 = str2;               // ok: pointer is const, but object is not const.
    std::cout << "static string after modification through ptr4: " << *getPtr() << "\n";

    return 0;
}