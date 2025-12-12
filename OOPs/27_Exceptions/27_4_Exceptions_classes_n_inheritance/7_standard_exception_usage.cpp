#include <iostream>
#include <stdexcept>
#include <exception>

int main()
{
    try
    {
        throw std::runtime_error("Something has gone wrong...!");
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
    }

    return 0;
}