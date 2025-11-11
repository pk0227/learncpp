#include <iostream>

//void nullify(int *p)    // p is a copy of the pointer passed in, so modifying p won't affect the original pointer.
void nullify(int*& p)     // p is a reference to the pointer passed in, so modifying p will affect the original pointer.
{
    p = nullptr;
}

int main()
{
    int i{10};
    int *ptr{&i};

    std::cout << "Before nullifying : ptr is " << (ptr ? "not-null" : "null") << "\n";

    nullify(ptr);

    std::cout << "After nullifying : ptr is " << (ptr ? "not-null" : "null") << "\n";
    
    return 0;
}