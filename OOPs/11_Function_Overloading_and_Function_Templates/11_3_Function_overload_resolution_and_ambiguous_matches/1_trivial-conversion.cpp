#include <iostream>

void func(const int)
{
    std::cout << "const int func" << std::endl;
}

/*
//causes ambiguity
void func(const int &)
{
    std::cout << "const int& func" << std::endl;
}
*/

void func(const double&)
{
    std::cout << "const double& func" << std::endl; 
}

int main(int argc, char **argv)
{
    int x{100};
    double d{3.6};

    func(1);
    func(2.4);
    func(1.2f);
    
    func(x);    // x trivially converted from int to const int
    func(d);    // d trivially converted from double to const double& (non-ref to ref conversion)

    return 0;
}