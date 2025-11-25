/*
    RETURNING LVALUE - OPTIONAL COPY ELISION (NRVO - Named Return Value Optimization)
    
    When a function returns a named local variable (lvalue), copy elision is optional but likely.
    However, semantic constructors ARE always evaluated (checked for availability).
    
    Key Observations:
    1. WITH copy elision enabled (default):
       - Semantics are EVALUATED (copy/move constructors must exist and be accessible)
       - Execution is ELIDED (constructors are not actually called)
       - If copy/move is deleted → compilation error
    
    2. WITH copy elision disabled (-fno-elide-constructors):
       - Semantics are EVALUATED and EXECUTED
       - Move constructor takes precedence over copy constructor
       - Move constructor is actually called (you'll see output)
    
    3. Constructor resolution order (if needed):
       - First choice: Move constructor
       - Fallback: Copy constructor
       - If both deleted → compilation error
*/

// SHORT SUMMARY:
// Semantic check in case of returning lvalue.
// If copy elision is enabled, semantics are evaluated, but not executed (execution is skipped/ignored).
// If copy elision is disabled, semantics are evaluated and executed based on precedence (Move Constructor takes precedence over Copy Constructor).
// In either case, as part of semantics evaluation, if semantic evaluation fails, compilation error occurs.
// Semantics evaluation rules in case of returning lvalue,
// 1. Move constructor takes precedence over copy constructor
// 2. Move constructor must not be deleted.
// 3. If copy semantics, destructor are impilicitly provided by compiler, Then Move semantics are also implicitly provided. - No issue.
// 4. If Move semantics are explicitly defined (either defined by user or requested to compiler using default keyword),
//        -- If copy semantics are NOT explicitly defined (either defined by user or requested to compiler using default keyword)
//           then, they are implicitly deleted.
//        -- Then, Move Constructor will execute irrespective of copy semantics defined/deleted, if execution is allowed(copy elision is disabled).
// 5. If Move semantics are NOT explicitly defined (either defined by user or requested to compiler using default keyword) and NOT deleted,
//    Then, copy constructor must NOT be deleted.

#include <iostream>

class Person
{
    std::string m_name{};
    
public:
    Person(std::string str="") : m_name{str}
    {
    }
    
//    Person(const Person&) = delete;

    Person(const Person& ref) : m_name{ref.m_name}
    {
        std::cout << "Copy Constructor...!\n";
    }

//    Person(Person&&) noexcept = delete;

    Person(Person&& ref) noexcept : m_name{ref.m_name}
    {
        std::cout << "Move Constructor...!\n";
    }

    const std::string& getName() const { return m_name; }
};

Person getPerson()
{
    Person temp{"The Flash"};
    return temp;
}

int main()
{
    Person p1{"Super Man"};
    Person p3{getPerson()};
    
    std::cout << p3.getName() << "\n";
    return 0;
}