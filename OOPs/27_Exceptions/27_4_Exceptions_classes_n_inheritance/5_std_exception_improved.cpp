#include <iostream>
#include <string>       
#include <cstddef>      
#include <limits>
#include <exception>    
#include <typeinfo>     // for typeid
#include <cxxabi.h>     // for demangling (GCC/Clang specific)

// Helper function to get readable exception type name
std::string getExceptionType(const std::exception& e)
{
    const char* mangledName = typeid(e).name();
    int status = 0;
    char* demangledName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
    
    std::string result;
    if (status == 0 && demangledName)
    {
        result = demangledName;
        free(demangledName);
    }
    else
    {
        result = mangledName;  // fallback to mangled name
    }
    
    return result;
}

// Helper function to provide user-friendly error messages
std::string getUserFriendlyMessage(const std::exception& e)
{
    std::string exceptionType = getExceptionType(e);
    
    // Map exception types to friendly messages
    if (exceptionType.find("length_error") != std::string::npos)
        return "The requested operation would exceed the maximum allowed size";
    else if (exceptionType.find("bad_alloc") != std::string::npos)
        return "Memory allocation failed - not enough memory available";
    else if (exceptionType.find("out_of_range") != std::string::npos)
        return "Attempted to access an element outside valid range";
    else if (exceptionType.find("invalid_argument") != std::string::npos)
        return "Invalid argument provided to function";
    else if (exceptionType.find("overflow_error") != std::string::npos)
        return "Arithmetic overflow occurred";
    else if (exceptionType.find("underflow_error") != std::string::npos)
        return "Arithmetic underflow occurred";
    else if (exceptionType.find("range_error") != std::string::npos)
        return "Result is outside the valid range";
    else if (exceptionType.find("runtime_error") != std::string::npos)
        return "Runtime error occurred";
    else if (exceptionType.find("logic_error") != std::string::npos)
        return "Logic error in program";
    else
        return "Unknown exception type";
}

int main()
{
    try
    {
        // Your code using standard library goes here
        // We'll trigger one of these exceptions intentionally for the sake of the example
        std::string str{};
        str.reserve(std::numeric_limits<std::size_t>::max());   // will trigger a std::length_error or allocation exception
    }
    catch(const std::exception& e)      
    {
        std::cerr << "========== EXCEPTION CAUGHT ==========\n";
        std::cerr << "Exception Type : " << getExceptionType(e) << "\n";
        std::cerr << "User Message   : " << getUserFriendlyMessage(e) << "\n";
        std::cerr << "Technical Info : " << e.what() << "\n";
        std::cerr << "=====================================\n";
    }

    return 0;
}
