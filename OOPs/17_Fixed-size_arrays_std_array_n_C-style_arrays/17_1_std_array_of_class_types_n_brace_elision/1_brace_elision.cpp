/*
 * Demonstration: Brace Elision with std::array of Class Types
 * 
 * Topic: Understanding double-braces {{ }} requirement for std::array initialization
 * 
 * Key Concepts:
 * 1. std::array is an aggregate containing a C-style array member
 * 2. Requires double braces: outer for std::array, inner for the C-style array
 * 3. Brace elision allows omitting inner braces in some cases (CTAD)
 * 4. Explicit type specification generally requires double braces
 */

#include <iostream>
#include <array>
#include <string_view>

// Simple aggregate struct to demonstrate array initialization
struct Student
{
    int m_ID{};
    std::string_view m_name{};
};

// Approach 1: CTAD (Class Template Argument Deduction) - Works!
// Compiler deduces type and size, brace elision allows single braces
/*
constexpr std::array arr{
        Student{1, "Rama"},
        Student{2, "Laxmana"},
        Student{3, "Hanuma"},
        Student{4, "Krishna"}
    };
*/

// Approach 2: Explicit type with single braces - Compilation Error!
// When type is explicit, brace elision doesn't apply the same way
/*
constexpr std::array<Student, 4> arr{           // compilation error
        {1, "Rama"},
        {2, "Laxmana"},
        {3, "Hanuma"},
        {4, "Krishna"}
};
*/

// Approach 3: Explicit type with double braces - Works!
// Outer braces for std::array, inner braces for the underlying C-style array
constexpr std::array<Student, 4> arr{{
        {1, "Rama"},
        {2, "Laxmana"},
        {3, "Hanuma"},
        {4, "Krishna"}
}};

// Search function that returns pointer to Student or nullptr
// Demonstrates constexpr std::array can be used in runtime functions
const Student* findStudentByID(int id)
{
    // Approach 1: Index-based loop using signed size (0Z literal for std::ptrdiff_t)
    for(auto i{0Z}; i < std::ssize(arr); i++)
    {
        if(arr[i].m_ID == id)
            return arr.data()+i;  // Return pointer to element
    }
    
    // Approach 2: Range-based for loop (commented out)
    // Both approaches work; index-based shown for demonstration
/*
    for(auto& s : arr)
    {
        if(s.m_ID == id)
            return &s;
    }
*/

    return nullptr;  // Student not found
}

int main()
{
    // Default value for when student is not found
    constexpr std::string_view nobody{"nobody"};

    // Test case 1: Search for existing student (ID = 2)
    const Student* s1{findStudentByID(2)};
    std::cout << (s1 ? s1->m_name : nobody) << "\n";  // Should print "Laxmana"

    // Test case 2: Search for non-existing student (ID = 5)
    const Student* s2{findStudentByID(5)};
    std::cout << (s2 ? s2->m_name : nobody) << "\n";  // Should print "nobody"

    return 0;
}