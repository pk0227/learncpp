#include <iostream>

int main()
{
/*
    char ch{};
    std::cout << "Enter char : \n";
    std::cin >> ch;
    std::cout << ch << "\n";

    std::cout << "Enter char : \n";
    std::cin >> ch;
    std::cout << ch << "\n";
*/

    char ch{};
    std::cout << "Enter char : \n";
    std::cin.get(ch);
    std::cout << ch << "\n";

    std::cout << "Enter char : \n";
    std::cin.get(ch);                       //capture space character in input if the given input is "a b".
    std::cout << ch << "\n";

    return 0;
}