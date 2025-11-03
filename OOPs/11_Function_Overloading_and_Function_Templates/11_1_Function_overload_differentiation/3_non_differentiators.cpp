/*  
typedefs, type aliases, const qualifier and return types 
are not function overloading differentiators.

These cause compilation errors like redifinition and ambiguating new declaration.
*/

#include <iostream>

typedef int Height;     //typedef
using Age = int;        //type alias

void print(int a)
{
    std::cout << "It is  : " << a << std::endl; 
}

/*
// causes redifintion compilation error
void print(const int a)
{
    std::cout << "It is  : " << a << std::endl; 
}
*/

/*
// causes redifintion compilation error
void print(Height a)                                    
{
    std::cout << "It is  : " << a << std::endl; 
}
*/

/*
// causes redifintion compilation error
void print(Age a)
{
    std::cout << "It is  : " << a << std::endl; 
}
*/

int add(int a, int b)
{
    std::cout << "int sum : ";
    return a+b;
}

/*
// causes ambiguating new declaration
double add(int a, int b)
{
    std::cout << "double sum : ";
    return a+b;
}
*/

int main(int argc, char **argv)
{
    print(10);
    std::cout << add(10,20) << std::endl;
    //std::cout << "\n" << add(10.2,20.3) << std::endl;
    return 0;
}