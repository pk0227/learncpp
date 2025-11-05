#include <iostream>

template <typename T, typename U>
T max(T x, U y)
{
    return (x < y) ? y : x;
}

int main()
{
    std::cout << max(1, 2.3) << '\n';   // returns 2 if return type is int(T) as 2.3 is narrowed to int by loosing 0.3
                                        // returns 2.3 if return type is double(U).
    return 0;               
}