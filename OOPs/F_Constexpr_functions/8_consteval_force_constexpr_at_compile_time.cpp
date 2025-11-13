#include <iostream>
#include <type_traits>
#include <optional>

consteval std::pair<std::optional<int>, std::optional<std::string_view>> consteval_func(const std::pair<std::optional<int>, std::optional<std::string_view>>& x)
{
    return x;
}

constexpr std::pair<std::optional<int>, std::optional<std::string_view>> compare(int x, int y)
{
    //if(std::is_constant_evaluated())
    if consteval
    {
        return {(x < y ? y : x), "Compile-time"};
    }

    return {(x < y ? x : y), "Run-time"};
}

int main()
{
    /*
    //constexpr auto [num, text] = consteval_func(compare(5, 6));       // Structured binding not allowed in constexpr before C++23
    auto [num, text] = consteval_func(compare(5, 6));
    if(num)
        std::cout << "Larger : " << *num << "\t";
    if(text)
        std::cout << "Context : " << *text << "\n";
    */
    
    constexpr auto res{consteval_func(compare(5, 6))};
    if(res.first)
        std::cout << "Larger : " << *res.first << "\t";
    if(res.second)
        std::cout << "Context : " << *res.second << "\n";
     
    return 0;
}