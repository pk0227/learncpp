#include <iostream>

int getValue()
{
    return 5;
}

const int& returnByConstRef(const int& obj)
{
    return obj;
}

int main()
{   
    const int& ref1{getValue()};     // direct binding of const ref to temp obj
    const int& ref2 {6};             // direct binding of const ref to temp obj   

    const int& ref3{returnByConstRef(11)};  //indirect binding which is unsafe and cause undefined behavior.

    std::cout << "ref1 : " << ref1 << "\n" << "ref2 : " << ref2 << "\n" << "ref3 : " << ref3 << "\n";
    return 0;
}