#include <iostream>
#include <iomanip>

int main()
{
    std::cout << static_cast<long long>(static_cast<double>(10000000000000001LL));
    
    int i{ 30000 };
    char c = i; // chars have range -128 to 127
    std::cout << static_cast<int>(c) << '\n';

    int i1{2};
    short s = i1; // convert from int to short
    std::cout << s << '\n';

    double d{0.1234};
    float f = d;
    std::cout << f << '\n';

    float f1 = 0.123456789;                          // double value 0.123456789 has 9 significant digits, but float can only support about 7
    std::cout << std::setprecision(9) << f1 << '\n'; // std::setprecision defined in iomanip header

    int i2{ 10 };
    float f2 = i2;
    std::cout << f << '\n';

    int i3 = 3.5;
    std::cout << i3 << '\n';

    return 0;
}