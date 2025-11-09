#include <iostream>

int main()
{
    double d{12.345};
    //int *ptr{static_cast<int*>(&d)};    // error: invalid ‘static_cast’ from type ‘float*’ to type ‘int*’
    
    int *ptr1{(int*)&d};        // C-style casting works fine but unsafe.
    
    int i{static_cast<int>(d)};
    
    int *ptr{reinterpret_cast<int*>(&d)};               // reinterpret_cast, which performs a bitwise reinterpretation of the address
    std::cout << "*ptr : " << *ptr << std::endl;
    
    double *dptr{reinterpret_cast<double*>(ptr)};       
    std::cout << "*dptr : " << *dptr << std::endl;
    
    std::cout << "sizeof(int) : " << sizeof(int) << std::endl;
    std::cout << "sizeof(double) : " << sizeof(double) << std::endl;
    std::cout << "sizeof(long int) : " << sizeof(long int) << std::endl;
    
    return 0;
}