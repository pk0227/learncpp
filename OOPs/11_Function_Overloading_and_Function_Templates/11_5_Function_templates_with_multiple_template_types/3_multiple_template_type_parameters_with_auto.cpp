#include <iostream>

template <typename T, typename U>
//T max(T x, U y)
//U max(T x, U y)

//std::common_type_t<T, U> max(T x, U y)
//auto max(T x, U y) -> std::common_type_t<T, U>
auto max(T x, U y)                      // ask compiler can figure out what the relevant return type is
{
    return (x < y) ? y : x;
}

int main()
{
    std::cout << max(1, 2.3) << '\n';   // returns 2 if return type is int(T) as 2.3 is narrowed to int by loosing 0.3
                                        // returns 2.3 if return type is double(U).
                                        // always returns 2.3 if return type is auto type deduction.

    std::cout << max(2.3, 1) << '\n';   // returns 2 if return type is int(U) as 2.3 is narrowed to int by loosing 0.3 
                                        // returns 2.3 if return type is double(T).
                                        // always returns 2.3 if return type is auto type deduction.
    return 0;               
}