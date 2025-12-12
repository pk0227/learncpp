#include <iostream>
#include <string>
#include <cstddef>      // for std::size_t
#include <limits>       // for std::numeric_limits
#include <exception>    // for std::exception and derived classes
#include <cxxabi.h>     // for abi::__cxa_demangle (GCC/Clang specific)

// Function to demangle the exception type name for better readability
// Returns human-readable exception type (e.g., "std::length_error" instead of "St12length_error")
std::string getDemangledName(const std::exception& e)
{
    // Get mangled type name from typeid
    const char* mangledName = typeid(e).name();
    int status{};
    
    // Demangle the name using GCC/Clang ABI function
    char* demangledName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);

    std::string result{};
    if(status == 0 && demangledName)  // Check if demangling was successful
    {
        result = demangledName;
        free(demangledName);  // Must free the memory allocated by __cxa_demangle
    }
    else  // Demangling failed, return original mangled name
    {
        result = mangledName;
    }

    return result;
}

int main()
{
    try
    {
        // Attempt to resize a string to maximum possible size
        // This will throw std::length_error because the size is too large
        std::string str{};
        str.resize(std::numeric_limits<std::size_t>::max());
    }
    catch(const std::length_error& e)  // Catch specific exception type first
    {
        // This handler catches std::length_error specifically
        // Useful when you want special handling for length/size related errors
        std::cerr << "SpecificError : " << e.what() << "\n";
        std::cerr << "DemangledName : " << getDemangledName(e) << "\n";
    }
    catch(const std::exception& e)  // Catch all other standard exceptions
    {
        // This handler catches any other std::exception or derived types
        // Should be placed after more specific handlers (exception hierarchy rule)
        std::cerr << "GenericError : " << e.what() << "\n";
    }

    return 0;
}