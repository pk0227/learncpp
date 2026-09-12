int main()
{
    int x { 5 };
    int& ref { x };         // okay: non-const lvalue reference bound to a modifiable lvalue

    const int y { 5 };
    // int& invalidRef { y };  // COMPILE ERROR: non-const lvalue reference cannot bind to a non-modifiable (const) lvalue
    // int& invalidRef2 { 0 }; // COMPILE ERROR: non-const lvalue reference cannot bind to an rvalue

    const int& validRef { y };  // OK: const lvalue reference can bind to a const lvalue
    const int& validRef2 { 0 }; // OK: const lvalue reference can bind to an rvalue (extends lifetime)

    return 0;
}