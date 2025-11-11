#include <iostream>

const int& getValue(bool printAddress = false)
{
    static const int id{1024};
    
    if(printAddress)
        std::cout << "address of id : " << &id << "\n";
    
    return id;
}

int main()
{
    int test_i{10};
    int& test_ref{test_i};

    std::cout << "address of test_i : " << &test_i << "\n";
    std::cout << "address of test_ref : " << &test_ref << "\n";

    auto ref{getValue(true)};                                           // ref has type int (const and reference dropped)
    ref = 2048;                                                      // modifying ref does not modify id
    std::cout << "address of ref : " << &ref << "\n" << "ref : " << ref << "\n" << "id : " << getValue() << "\n";

    return 0;
}