#include <iostream>

int main()
{
    //int& invalidRef;   // error: references must be initialized

    int x { 5 };
    int& ref1 { x }; // okay: reference to int is bound to int variable

    // int& ref2 {10}; // initial value of reference to non-const must be an lvalue
    const int& ref2 {10};

    std::cout << ref1 << "\n" << ref2 << "\n";
    
    return 0;
}