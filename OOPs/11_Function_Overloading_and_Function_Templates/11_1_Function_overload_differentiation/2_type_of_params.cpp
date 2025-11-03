#include <iostream>

//function-1
int add(int a, int b)
{
    return a+b;
}

//function-2
double add(double x, double y)
{
    return x+y;
}

//function-3
double add(double x, int b)
{
    return x+b;
}

//function-4
double add(int b, double x)
{
    return b+x;
}

// all above functions are example of type of params
// functions-3 and 4 are examples of order of the functions too.

int main()
{
    std::cout << "func-1 : " << add(2,3) << std::endl;
    std::cout << "func-2 : " << add(2.5,3.5) << std::endl;
    std::cout << "func-1 : " << add(2.5,3) << std::endl;
    std::cout << "func-1 : " << add(2,3.6) << std::endl;

    return 0;
}