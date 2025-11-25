/*
    COMPREHENSIVE GUIDE TO LVALUES AND RVALUES IN C++
    
    This file covers essential concepts about lvalues, rvalues, and rvalue references
    with clear examples and explanations.
*/

#include <iostream>

// ============================================================================
// SECTION 1: BASIC DEFINITIONS
// ============================================================================

/*
    LVALUE (Locator Value):
    - Has a persistent memory address (identifiable location)
    - Can appear on the left side of assignment
    - Examples: variables, array elements, dereferenced pointers
    - Persists beyond a single expression
    
    RVALUE (Read Value):
    - Temporary value without a persistent address
    - Cannot appear on the left side of assignment (non-modifiable)
    - Examples: literals, temporary objects, results of expressions
    - Does not persist beyond the expression that uses it
    
    MODERN C++ VALUE CATEGORIES (C++11 onwards):
    - glvalue (generalized lvalue): lvalue or xvalue
    - rvalue: prvalue or xvalue
    - lvalue: traditional lvalue
    - xvalue (expiring value): result of std::move, cast to rvalue reference
    - prvalue (pure rvalue): traditional rvalue (literals, temporaries)
*/

// ============================================================================
// SECTION 2: LVALUE vs RVALUE EXAMPLES
// ============================================================================

void section2_lvalue_rvalue_basics()
{
    std::cout << "\n=== SECTION 2: LVALUE vs RVALUE BASICS ===\n";
    
    int x = 5;          // x is an lvalue, 5 is an rvalue
    int y = x + 1;      // y is an lvalue, (x + 1) is an rvalue
    
    // x = 10;          // OK: x is an lvalue (can be on left side)
    // 5 = x;           // ERROR: 5 is an rvalue (cannot be on left side)
    // x + 1 = 20;      // ERROR: (x + 1) is an rvalue
    
    int* ptr = &x;      // OK: x is an lvalue, has an address
    // int* ptr2 = &5;  // ERROR: 5 is an rvalue, no address
    
    std::cout << "x = " << x << ", y = " << y << "\n";
}

// ============================================================================
// SECTION 3: FUNCTION RETURN VALUES
// ============================================================================

int returnByValue()
{
    return 42;          // Returns a prvalue (temporary)
}

int& returnByReference(int& ref)
{
    return ref;         // Returns an lvalue reference
}

void section3_function_returns()
{
    std::cout << "\n=== SECTION 3: FUNCTION RETURN VALUES ===\n";
    
    // returnByValue() returns an rvalue (temporary)
    int a = returnByValue();    // OK: copy the temporary
    // returnByValue() = 10;    // ERROR: cannot assign to rvalue
    
    int x = 100;
    // returnByReference(x) returns an lvalue
    returnByReference(x) = 200; // OK: can assign to lvalue
    std::cout << "x after assignment through lvalue reference: " << x << "\n";
}

// ============================================================================
// SECTION 4: LVALUE AND RVALUE REFERENCES
// ============================================================================

void section4_references()
{
    std::cout << "\n=== SECTION 4: LVALUE AND RVALUE REFERENCES ===\n";
    
    int x = 10;
    
    // LVALUE REFERENCES (T&)
    int& lref = x;              // OK: lvalue reference binds to lvalue
    // int& lref2 = 5;          // ERROR: non-const lvalue ref cannot bind to rvalue
    const int& clref = 5;       // OK: const lvalue ref CAN bind to rvalue (lifetime extension!)
    const int& clref2 = x + 1;  // OK: const lvalue ref extends temporary's lifetime
    
    // RVALUE REFERENCES (T&&)
    int&& rref = 42;            // OK: rvalue reference binds to rvalue
    // int&& rref2 = x;         // ERROR: rvalue ref cannot bind to lvalue
    int&& rref3 = x + 5;        // OK: rvalue ref binds to temporary
    
    std::cout << "lref = " << lref << ", clref = " << clref << "\n";
    std::cout << "rref = " << rref << ", rref3 = " << rref3 << "\n";
}

// ============================================================================
// SECTION 5: CRITICAL CONCEPT - NAMED RVALUE REFERENCES ARE LVALUES!
// ============================================================================

void section5_named_rvalue_refs_are_lvalues()
{
    std::cout << "\n=== SECTION 5: NAMED RVALUE REFERENCES ARE LVALUES ===\n";
    
    int&& rref = 100;           // rref is of type "rvalue reference to int"
                                // BUT rref itself is an LVALUE (it has a name!)
    
    int& lref = rref;           // OK: rref is an lvalue, can bind to lvalue ref
    // int&& rref2 = rref;      // ERROR: cannot bind rvalue ref to lvalue (rref is lvalue!)
    int&& rref2 = std::move(rref); // OK: std::move converts lvalue to xvalue (rvalue)
    
    std::cout << "Key insight: variable 'rref' has type 'int&&' but is an lvalue!\n";
    std::cout << "rref = " << rref << ", can take its address: " << &rref << "\n";
}

// ============================================================================
// SECTION 6: FUNCTION OVERLOADING WITH LVALUE AND RVALUE REFERENCES
// ============================================================================

void process(const int& lref)
{
    std::cout << "  Called: process(const int& lref) - lvalue binding, value = " << lref << "\n";
}

void process(int&& rref)
{
    std::cout << "  Called: process(int&& rref) - rvalue binding, value = " << rref << "\n";
}

void section6_function_overloading()
{
    std::cout << "\n=== SECTION 6: FUNCTION OVERLOADING ===\n";
    
    int x = 10;
    
    process(x);                 // Calls lvalue version (x is lvalue)
    process(42);                // Calls rvalue version (42 is rvalue)
    process(x + 5);             // Calls rvalue version (x + 5 is temporary/rvalue)
    
    int&& rref = 20;
    process(rref);              // Calls lvalue version! (rref is lvalue despite type)
    process(std::move(rref));   // Calls rvalue version (std::move casts to rvalue)
    process(std::move(x));      // Calls rvalue version (std::move casts to rvalue)
}

// ============================================================================
// SECTION 7: std::move - CONVERTING LVALUES TO RVALUES
// ============================================================================

void section7_std_move()
{
    std::cout << "\n=== SECTION 7: std::move USAGE ===\n";
    
    /*
        std::move() doesn't actually "move" anything!
        It's just a cast that converts an lvalue to an xvalue (rvalue).
        This allows binding to rvalue references, enabling move semantics.
    */
    
    int x = 100;
    // x is an lvalue
    std::cout << "x before move: " << x << "\n";
    
    int&& rref = std::move(x);  // std::move(x) converts x to rvalue
                                // Now we can bind it to rvalue reference
    
    std::cout << "x after move: " << x << " (value unchanged - std::move is just a cast!)\n";
    std::cout << "rref: " << rref << "\n";
    
    // Note: std::move doesn't modify x, it just allows treating it as an rvalue
    // The actual "move" happens when a move constructor/assignment is called
}

// ============================================================================
// SECTION 8: LIFETIME EXTENSION
// ============================================================================

void section8_lifetime_extension()
{
    std::cout << "\n=== SECTION 8: LIFETIME EXTENSION ===\n";
    
    /*
        When you bind a const lvalue reference or rvalue reference to a temporary,
        the lifetime of that temporary is extended to match the reference's lifetime.
    */
    
    const int& clref = 42;      // Temporary 42's lifetime extended
    int&& rref = 100;           // Temporary 100's lifetime extended
    
    std::cout << "clref = " << clref << " (temporary's lifetime extended)\n";
    std::cout << "rref = " << rref << " (temporary's lifetime extended)\n";
    
    // Without lifetime extension, these would be dangling references!
}

// ============================================================================
// SECTION 9: DANGLING REFERENCES (COMMON PITFALL)
// ============================================================================

const int& danglingReference()
{
    int x = 42;
    return x;               // DANGER: returning reference to local variable!
}

int&& danglingRvalueRef()
{
    return 42;              // DANGER: returning rvalue ref to temporary!
}

void section9_dangling_references()
{
    std::cout << "\n=== SECTION 9: DANGLING REFERENCES (PITFALLS) ===\n";
    
    // These are DANGEROUS and lead to undefined behavior:
    // const int& bad = danglingReference();    // Dangling reference!
    // int&& bad2 = danglingRvalueRef();        // Dangling reference!
    
    std::cout << "WARNING: Never return references to local variables!\n";
    std::cout << "WARNING: Be careful with lifetime of temporaries!\n";
}

// ============================================================================
// SECTION 10: PRACTICAL USE CASE - MOVE SEMANTICS PREVIEW
// ============================================================================

class Resource
{
    int* data;
public:
    Resource(int value) : data(new int(value))
    {
        std::cout << "  Resource constructed with value " << *data << "\n";
    }
    
    ~Resource()
    {
        std::cout << "  Resource destroyed\n";
        delete data;
    }
    
    // Copy constructor (expensive for large resources)
    Resource(const Resource& other) : data(new int(*other.data))
    {
        std::cout << "  Resource COPIED (expensive!)\n";
    }
    
    // Move constructor (cheap - just transfer ownership)
    Resource(Resource&& other) noexcept : data(other.data)
    {
        other.data = nullptr;
        std::cout << "  Resource MOVED (cheap!)\n";
    }
    
    int getValue() const { return data ? *data : 0; }
};

Resource createResource()
{
    return Resource(42);
}

void useResource(Resource&& res)    // Accepts rvalue only
{
    std::cout << "  Using resource with value: " << res.getValue() << "\n";
}

void section10_move_semantics_preview()
{
    std::cout << "\n=== SECTION 10: MOVE SEMANTICS PREVIEW ===\n";
    
    std::cout << "Creating resource from function return (rvalue):\n";
    Resource r1 = createResource();     // May be elided, but move is preferred over copy
    
    std::cout << "\nPassing lvalue (requires copy or std::move):\n";
    Resource r2(100);
    // useResource(r2);                 // ERROR: cannot bind lvalue to rvalue ref
    useResource(std::move(r2));         // OK: std::move converts to rvalue
    
    std::cout << "\nPassing rvalue directly:\n";
    useResource(Resource(200));         // OK: temporary is rvalue
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main()
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  COMPREHENSIVE LVALUE/RVALUE GUIDE\n";
    std::cout << "========================================\n";
    
    section2_lvalue_rvalue_basics();
    section3_function_returns();
    section4_references();
    section5_named_rvalue_refs_are_lvalues();
    section6_function_overloading();
    section7_std_move();
    section8_lifetime_extension();
    section9_dangling_references();
    section10_move_semantics_preview();
    
    std::cout << "\n========================================\n";
    std::cout << "  END OF DEMONSTRATION\n";
    std::cout << "========================================\n\n";
    
    return 0;
}
