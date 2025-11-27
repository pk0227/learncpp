#include <iostream>
#include <string>
#include <utility>

template <typename T>
void userSwap(T& a, T& b)
{
/*
    T temp;
    temp = a;
    a = b;
    b = temp;
*/
    T temp = {std::move(a)};    // invokes move constructor
    a      = std::move(b);      // invokes move assignment
    b      = std::move(temp);   // invokes move assignment
}

int main()
{
    std::string s1{"Rama"};
    std::string s2{"Seetha"};

    std::cout << "s1 : " << s1 << "\n";
    std::cout << "s2 : " << s2 << "\n";

    userSwap(s1, s2);

    std::cout << "s1 : " << s1 << "\n";
    std::cout << "s2 : " << s2 << "\n";

    return 0;
}