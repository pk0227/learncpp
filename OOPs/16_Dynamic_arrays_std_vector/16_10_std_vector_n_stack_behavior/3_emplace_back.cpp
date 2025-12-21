/**
 * Demonstration of emplace_back() vs push_back()
 * 
 * Key Concepts:
 * 1. push_back(obj): Constructs object first, then copies/moves it into vector
 * 2. emplace_back(args...): Constructs object directly in vector (in-place construction)
 * 3. emplace_back() can forward constructor arguments directly
 * 4. emplace_back() works with explicit constructors
 * 
 * Performance benefits:
 * - emplace_back() avoids creating temporary objects
 * - Single construction vs construction + move/copy
 * - More efficient for complex objects
 */

#include <iostream>
#include <vector>
#include <string>
#include <string_view>

class Foo
{
    std::string m_str{};
    int m_i{};
public:
    // Regular constructor taking two parameters
    Foo(std::string_view str, int i) : m_str{str}, m_i{i}
    { }

    // Explicit constructor - prevents implicit conversions
    // Can only be called explicitly, not through implicit conversion
    explicit Foo(int x) : m_str{}, m_i{x}
    { }
};

void testExplicit(Foo obj)
{
    std::cout << "testExplicit called\n";
}

int main()
{
    std::vector<Foo> stack{};

    // 1. Using push_back() with an existing object
    Foo f{"abc", 12};  // Construct Foo object
    stack.push_back(f);     // Copy f into vector
    stack.emplace_back(f);  // Also copies f (no advantage here)

    // 2. Using push_back() with temporary object (brace initialization)
    stack.push_back({"xyz", 23});  // Creates temporary Foo, then moves it
    
    // This doesn't work with push_back - can't pass separate arguments
    // stack.push_back("mno", 34);  // ERROR: push_back expects Foo object
    
    // emplace_back() forwards arguments directly to Foo constructor
    stack.emplace_back("mno", 45);  // Constructs Foo("mno", 45) in-place
    
    // 3. Working with explicit constructors
    // This doesn't work - brace initialization with explicit constructor
    // stack.push_back({2});  // ERROR: explicit constructor prevents this
    
    // emplace_back() can call explicit constructors directly
    stack.emplace_back(2);  // Calls explicit Foo(int) constructor in-place
    
    std::cout << "-----\n";
    
    return 0;
}