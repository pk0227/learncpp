#include <iostream>
#include <string>

int main()
{
    try
    {
        throw 'E';  // Throwing a char exception
    }
    catch(int e)    // No numeric promotion to int
    {
        std::cout << "int catch block : " << e << "\n";
    }
    catch(char e)   // Catching char exception. If char exception block isn't present, it wont promote/convert to any other type. Terminates with uncaught exception.
    {
        std::cout << "char catch block : " << e << "\n";
    }
    catch(double e) // No numeric conversion to double
    {
        std::cout << "double catch block : " << e << "\n";
    }
    catch(const std::string& e) // char does not promote to std::string
    {
        std::cout << "std::string catch block : " << e << "\n";
    }
    
    return 0;
}