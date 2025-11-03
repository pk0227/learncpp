#include <iostream>
#include <string_view>

/*
void print(std::string_view sv="Hello", double d=10.0)
{
    std::cout << "sv : " << sv << '\n';
}
*/

void print(const char *str = "Hello", double d=10.0)
{
    std::cout << "str : " << str << '\n';
}

/*
void print(char ch = 'A', double d = 10.0)
{
    std::cout << "ch : " << ch << '\n';
}
*/

int main()
{
    print();           // okay: both arguments defaulted
    print("Macaroni"); // okay: d defaults to 10.0
    //print(20.0);       // error: does not match above function (cannot skip argument for sv)

    return 0;
}