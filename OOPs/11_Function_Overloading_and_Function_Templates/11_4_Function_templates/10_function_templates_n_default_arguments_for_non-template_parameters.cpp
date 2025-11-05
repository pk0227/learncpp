#include <iostream>

template <typename T>
void print(T val, int times=1)
{
    while(times)
    {
        std::cout << val << '\n';
        times--;
    }
}

int main()
{
    print(5);
    print('A', 3);
    return 0;
}