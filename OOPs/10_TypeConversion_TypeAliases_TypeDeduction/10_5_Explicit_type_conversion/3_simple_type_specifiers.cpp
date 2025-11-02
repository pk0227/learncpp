#include <iostream>

int main()
{
    unsigned char c{'a'};
    //std::cout << unsigned int{c} << '\n';   //only single-word type names are allowed (i.e simple type specifiers)

    //work around 
    using uint = unsigned int;
    std::cout << uint { c } << '\n';

    return 0;
}