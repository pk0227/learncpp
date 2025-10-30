#include <iostream>

void printInt(int i)
{
    std::cout << i << '\n';
}

int main()
{
    printInt(2);
    short int si{3};
    printInt(si);
    printInt('a');
    printInt(true);
    
    return 0;
}