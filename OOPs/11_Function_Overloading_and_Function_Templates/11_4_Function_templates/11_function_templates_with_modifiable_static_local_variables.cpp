#include <iostream>

template <typename T>
void printIDandVal(T val)
{
    static int id{0};
    std::cout << "id : " << ++id << "\tval : " << val << '\n';
}

int main()
{
    printIDandVal(100);
    printIDandVal(200);
    printIDandVal(1.1);

    return 0;
}