#include <iostream>

class IceCream
{
    std::string m_flavor{};
public:
    static int icecream_count;          // pure declaration

    IceCream(const std::string_view flavor) : m_flavor{flavor}
    {
        icecream_count++;
    }
};

int IceCream::icecream_count{123};      // define and initialize

int main()
{
    // accessing static variable without object
    std::cout << "default initialized icecream count : " << IceCream::icecream_count << "\n";
    IceCream::icecream_count = 0;
    std::cout << "Before object creation, icecream count : " << IceCream::icecream_count << "\n";

    IceCream i1{"cherry"};
    IceCream i2{"chocolate"};

    std::cout << "current icecream count : " << IceCream::icecream_count << "\n";
    // accessing static variable with object
    std::cout << "current icecream count : " << i1.icecream_count << "\n";

    return 0;
}