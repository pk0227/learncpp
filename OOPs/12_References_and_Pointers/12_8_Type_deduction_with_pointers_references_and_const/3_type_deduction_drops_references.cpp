#include <iostream>

int& getValue()
{
    static int id{1024};
    std::cout << "address of id : " << &id << "\n";
    return id;
}

int main()
{
    int test_i{10};
    int& test_ref{test_i};

    std::cout << "address of test_i : " << &test_i << "\n";
    std::cout << "address of test_ref : " << &test_ref << "\n";

    auto ref{getValue()};                                       // ref has type int (reference dropped)                  
    std::cout << "address of ref : " << &ref << "\n";

    return 0;
}