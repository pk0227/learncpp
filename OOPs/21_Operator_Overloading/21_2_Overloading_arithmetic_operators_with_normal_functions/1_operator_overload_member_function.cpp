#include <iostream>
#include "Cents.hpp"

int main()
{
    Cents c1{6};
    Cents c2{10};

    Cents sum{c1 + c2};
    Cents sub{c2-c1};

    std::cout << "Total Cents : " << sum.getCents() << "\n";
    std::cout << "Total Cents : " << sub.getCents() << "\n";

    return 0;
}