/**
 * Demonstration of std::vector initialization - List vs Direct initialization
 * 
 * Key Concepts:
 * 1. Brace initialization {} creates a vector with specified elements (list constructor)
 * 2. Parenthesis initialization () creates a vector with specified size (size constructor)
 * 3. Class member initialization restrictions with direct initialization
 */

#include <iostream>
#include <vector>

// Class demonstrating list initialization for member vector
class Foo
{
    // List initialization: Creates a vector with one element (value 10)
    std::vector<int> v1{10};  // v1 contains: [10]
public:
    void getElements()
    {
        // Range-based for loop to iterate through vector elements
        for(const auto& i : v1)
            std::cout << i << " ";

        std::cout << "\n";
    }
};

// Class demonstrating workaround for direct initialization in member variables
class Cls
{
    // Direct initialization v1(20) is NOT allowed for non-static data members
    // Workaround: Use list initialization with a temporary vector object
    // Creates a vector with 20 default-initialized elements (all zeros)
    std::vector<int> v1{std::vector<int>(20)};  // v1 contains: [0, 0, 0, ..., 0] (20 elements)
public:
    void getElements()
    {
        for(const auto& i : v1)
            std::cout << i << " ";

        std::cout << "\n";
    }
};

int main()
{
    // List initialization: Creates vector with ONE element (value 10)
    std::vector<int> v1{10};       // v1.size() = 1, contains: [10]
    
    // Direct initialization: Creates vector with 10 default-initialized elements
    std::vector<int> v2(10);       // v2.size() = 10, contains: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

    // List initialization with user-defined type: Creates vector with 12 Foo objects
    std::vector<Foo> v3{12};       // v3.size() = 12
    
    // const vector: The vector itself is const, not the element type
    const std::vector<int> v4{10, 20, 30, 40, 50};  // Cannot modify v4, but elements are int
    
    // This is NOT allowed: std::vector cannot have const element type
    // std::vector<const int> would cause compilation errors on modification operations

    // Display sizes to demonstrate the difference
    std::cout << "v1.size() : " << v1.size() << "\n";  // Output: 1
    std::cout << "v2.size() : " << v2.size() << "\n";  // Output: 10
    std::cout << "v3.size() : " << v3.size() << "\n";  // Output: 12

    // Access first Foo object and display its vector elements
    v3[0].getElements();  // Output: 10

    return 0;
}