#include <iostream>

void print(std::string_view sv)
{
    std::cout << sv << "\n";
}

int main()
{
    std::string str{"Hello"};
    print(str);
    
    return 0;
}