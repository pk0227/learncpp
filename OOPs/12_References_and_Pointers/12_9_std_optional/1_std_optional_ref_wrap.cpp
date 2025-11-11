#include <iostream>
#include <optional>

void print(std::optional<std::reference_wrapper<int>> e=std::nullopt)   // std::optional mimics its reference with help of reference_wrapper
{
    if(e)
        std::cout << "Result with ref_wrap : " << e->get() << "\n";
    else
        std::cout << "Result with ref_wrap : print failed\n";
}

/*
void print(std::optional<int> e=std::nullopt)
{
    if(e)
        std::cout << "Result with optional : " << *e << "\n";
    else
        std::cout << "Result with optional : print failed\n";
}
*/
/*
void print(const std::optional<int>& e=std::nullopt)
{
    if(e)
        std::cout << "Result with optional : " << *e << "\n";
    else
        std::cout << "Result with optional : print failed\n";
}
*/
/*
void print(int i)
{
        std::cout << "i : " << i << "\n";
}
*/

int main()
{
    int i{}, j{100};
    print({});
    print(j);
    //print(200);     // will error with std::optional mimic reference
    //print(static_cast<int>(200));     // will error with std::optional mimic reference

    return 0;
}