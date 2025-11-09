#include <iostream>

int g_x{6};

int main()
{
    constexpr int& ref1{g_x};       // ok, can bind to global

    static int st_int{10};
    constexpr int& ref2{st_int};    // ok, can bind to static local

    int local_int{20};
    //constexpr int& ref3{local_int};   // compile error: can't bind to non-static object

    static const int st_const_int_var{100};
    constexpr const int& ref4{st_const_int_var};    // needs both constexpr and const

    std::cout << "Thanks!\n";
    return 0;
}