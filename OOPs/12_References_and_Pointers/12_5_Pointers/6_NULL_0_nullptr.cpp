#include <iostream>

void print(int x)
{
    std::cout << "print(int) : " << x << "\n"; 
}

void print(int *ptr)
{
    std::cout << "print(int*) : ptr is " << (ptr ? "not-null" : "null") << "\n";
}

void print(std::nullptr_t ptr)
{
    std::cout << "print(std::nullptr_t) : ptr is " << (ptr ? "not-null" : "null") << "\n";
}

int main()
{
    int *ptr{NULL};
    print(ptr);         // calls print(int *)
    print(0);           // calls print(int)

    //print(NULL);    // error: call of overloaded 'print(NULL)' is ambiguous

    print(nullptr);     // calls print(std::nullptr_t)
    
    return 0;
}