/*
    RETURNING PRVALUE - MANDATORY COPY ELISION (C++17)
    
    When a function returns a prvalue (pure rvalue like Person{"The Flash"}), 
    C++17 mandates copy elision - the object is directly constructed in the caller's space.
    
    Key Observations:
    1. Equivalent to: Person p3{"The Flash"} (direct construction at call site)
    2. NO semantic constructor evaluation - copy/move constructors are NOT checked
    3. Works even if copy/move constructors are deleted or private
    4. The -fno-elide-constructors flag has NO effect (elision is mandatory, not optional)
    5. This is NOT an optimization - it's part of the language specification
*/

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
    return Person{"The Flash"};     // Returns prvalue - triggers mandatory copy elision
}

int main()
{
    Person p1{"Super Man"};
    Person p3{getPerson()};     // Direct construction - equivalent to: Person p3{"The Flash"}
                                // No copy/move constructor called or checked
                                // Works even if copy/move constructors are deleted
    
    std::cout << p3.getName() << "\n";
    
    return 0;
}