#include <iostream>
#include <string>       // for this example
#include <cstddef>      // for std::size_t
#include <limits>
#include <exception>    // for std::exception

int main()
{
    try
    {
        // Your code using standard library goes here
        // We'll trigger one of these exceptions intentionally for the sake of the example
        std::string str{};
        str.reserve(std::numeric_limits<std::size_t>::max());   // will trigger a std::length_error or allocation exception
    }
    catch(const std::exception& e)      // This handler will catch std::exception and all the derived exceptions too
    {
        std::cerr << "Error : " << e.what() << "\n";
    }

    return 0;
}