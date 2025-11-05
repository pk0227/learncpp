#include <iostream>
#include <type_traits> // for std::common_type_t

template <typename T, typename U>
//auto max(T x, U y);                 // error: use of 'auto max(T, U) [with T = int; U = double]' before deduction of 'auto'                  
//std::common_type_t<T, U> max(T x, U y);
auto max(T x, U y) -> std::common_type_t<T, U>; 

int main()
{
    std::cout << max(1, 2.3) << '\n';   

    std::cout << max(2.3, 1) << '\n';
    
    return 0;               
}

template <typename T, typename U>
//auto max(T x, U y)
//std::common_type_t<T, U> max(T x, U y)
auto max(T x, U y) -> std::common_type_t<T, U>
{
    return (x < y) ? y : x;
}