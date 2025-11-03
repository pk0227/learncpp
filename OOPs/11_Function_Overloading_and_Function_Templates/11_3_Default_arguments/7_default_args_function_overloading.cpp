#include <iostream>
#include <string_view>

void print(std::string_view sv)
{
    std::cout << sv << '\n';
}

void print(char ch = 'A')
{
    std::cout << ch << '\n';
}

int main()
{
    print("Hello world");
    print('C');
    print();
    return 0;
}