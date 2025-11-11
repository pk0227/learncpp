#include <iostream>

int g_x{6};

int main()
{
    constexpr int& ref1{g_x};       // ok, can bind to global
    ref1 = 7;                       // modifies g_x through ref1. constexpr doesnt apply const to the object being referred.
                                    // constexpr allows the reference to be used in a constant expression.

    static int st_int{10};
    constexpr int& ref2{st_int};    // ok, can bind to static local
    ref2 = 15;                      // modifies st_int through ref2

    int local_int{20};
    //constexpr int& ref3{local_int};   // compile error: can't bind to non-static object

    static const int st_const_int_var{100};
    constexpr const int& ref4{st_const_int_var};    // needs both constexpr and const. Because constexpr allows the reference to be used in a constant expression.
                                                    // const is needed because st_const_int_var is const.
                                                    // When defining a constexpr reference to a const variable, we need to apply both constexpr (which applies to the reference) and const (which applies to the type being referenced).
                                                    
    //ref4 = 200;                                   // error: cannot modify a const object.

    std::cout << "Thanks!\n";
    return 0;
}