#include <iostream>
#include <string>

int main()
{
    try
    {
        throw 5;  // Throwing an int exception
    }
    catch(char e)  // No narrowing conversion to char
    {
        std::cout << "char catch block : " << e << "\n";
    }
    catch(double e) // No numeric conversion to double
    {
        std::cout << "double catch block : " << e << "\n";
    }
    catch(int e)   // Catching int exception. If int exception block isn't present, it wont convert to any other type. Terminates with uncaught exception.
    {
        std::cout << "int catch block : " << e << "\n";
    }
    catch(const std::string& e) // int does not convert to std::string
    {
        std::cout << "std::string catch block : " << e << "\n";
    }
    
    return 0;
}