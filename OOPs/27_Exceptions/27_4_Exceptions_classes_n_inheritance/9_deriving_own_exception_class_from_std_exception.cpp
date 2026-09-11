#include <iostream>
#include <string>
#include <string_view>
#include <exception>    // for std::exception base class
#include <cstddef>      // for std::size_t
#include <utility>      // for std::move

// Custom exception class derived from std::runtime_error (which itself derives from std::exception)
// This allows our exceptions to be caught by handlers expecting std::exception or std::runtime_error
// Using std::runtime_error is preferred over std::exception when you need to store error messages
// because std::runtime_error already handles string storage and what() implementation
class ArrayException : public std::runtime_error
{
public:
    // Constructor takes error message and forwards it to std::runtime_error's constructor
    // std::runtime_error will store the message and make it available via what()
    // Using std::move avoids unnecessary string copying
    ArrayException(std::string err="") : std::runtime_error{std::move(err)}
    { }
    // Note: No need to override what() - std::runtime_error already provides it
};

// Template class for a generic fixed-size array
// T: type of elements, size: number of elements (compile-time constant)
template <typename T, std::size_t size>
class GenericArray
{
    T m_array[size];  // Fixed-size array of type T
public:
    // Returns the size of the array
    int getSize() const { return size; }
    
    // Overloaded subscript operator with bounds checking
    // Throws ArrayException if index is out of valid range [0, size)
    T& operator[](std::size_t index) 
    {
        // Check if index is out of bounds
        // Note: index < 0 is always false for unsigned std::size_t, but kept for clarity
        if(index < 0 || index >= size)
            throw ArrayException{"Invalid Index"};  // Throw custom exception
        return m_array[index]; 
    }
};

int main()
{
    // Create an array of 10 doubles (all default-initialized to 0.0)
    GenericArray<double, 10> gad{};
    std::size_t index{10};  // Invalid index (valid range is 0-9)

    try
    {
        // Attempt to access element at index 10 (out of bounds)
        // This will throw ArrayException from operator[]
        std::cout << "gad[" << index << "] : " << gad[index] << "\n";
    }
    catch(const ArrayException& e)  // Catch our custom exception
    {
        // Handle the exception by printing error message
        // e.what() returns the custom message "Invalid Index"
        std::cerr << "Error : " << e.what() << "\n";
    }

    return 0;
}