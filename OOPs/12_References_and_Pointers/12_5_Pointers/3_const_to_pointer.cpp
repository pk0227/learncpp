#include <iostream>

int main()
{
    const int x { 5 }; // x is now const
    //int* ptr1 { &x };   // compile error: cannot convert from const int* to int*

    const int* ptr1{&x};

    int y{10};
    int z{20};

    int* const ptr2{&y};    
    // ptr2 = &z;              // error: assignment of read-only variable ‘ptr2’

    *ptr2 = 123;

    const int* const ptr3{&z};
    // *ptr3 = 111;                // error: assignment of read-only location ‘*(const int*)ptr3’

    std::cout << "const to pointers..!\n";

    return 0;
}